# Troubleshooting & porting guide

This document is the post-mortem of everything that broke while reconstructing
this firmware, and a checklist for anyone porting this per-key/per-layer
VialRGB + EEPROM-persistence setup to a **different split keyboard**. If
you're just using this repo as-is on the same Sofle Choc Pro, you don't need
to read this — see `README.md` instead.

## 1. How the original source was lost (repo-level lesson)

A firmware source directory was added to the git repo as a **gitlink**
(a `160000`-mode tree entry, like a submodule) pointing at a commit on a
local disk, without a matching `.gitmodules` entry and without ever pushing
that commit anywhere. Git doesn't store a gitlink's content — only a
reference to "look it up elsewhere." Once the workspace containing that
commit was deleted, the reference pointed at nothing, and the content was
gone for good, with no way to recover it from the git repo itself.

**Lesson: never add a keyboard firmware source tree as a gitlink/submodule
unless it's a public, permanently-pushed upstream repo you don't modify.**
If you fork/patch it, vendor the modified files as plain tracked files (like
`keymaps/vial/` and `patches/` in this directory) so `git log`/`git diff`
actually shows their content, and a `git clone` of this repo alone is enough
to reconstruct everything except the untouched upstream QMK tree (which is
public and re-clonable by design).

## 2. Bugs found while rebuilding Direct-mode + split RGB

All of these were found through the same method: read the HID debug report
(`RGB_DEBUG_CMD`, `0xF0`) to confirm internal firmware state (mode, enable,
buffer contents, HSV→RGB conversion) was correct, notice the physical LEDs
still didn't match that state, and narrow down *where between "correct
buffer" and "photons coming out of the LED" the signal got lost*. If you're
debugging a similar board and effects render for a single-piece keyboard but
not a split one, this is the order to check things in.

### 2a. VialRGB Direct mode's "am I done rendering?" check ignores split boards

**File:** `quantum/rgb_matrix/animations/vialrgb_direct_anim.h` (stock QMK/Vial,
not this keymap — patched via `patches/vialrgb_direct_anim.h`)

The effect function determines whether it still has LEDs left to process
this frame with:

```c
return led_max < RGB_MATRIX_LED_COUNT;
```

`RGB_MATRIX_LED_COUNT` is the keyboard's *total* LED count (both halves
combined — 60 on this board). But on a split keyboard with `RGB_MATRIX_SPLIT`
defined, `rgb_matrix_get_limits()` caps `led_max` for the **left** half at
its own LED count (`k_rgb_matrix_split[0]`, 30 here) — it never lets `led_max`
reach the combined total. So on the left half, `30 < 60` is *always* true:
the effect never reports "finished," the render task state machine
(`rgb_task_render` → `RENDERING` → `FLUSHING`) never reaches the `FLUSHING`
step, and `rgb_matrix_update_pwm_buffers()` (the actual driver write) never
runs — even though every intermediate value (mode, HSV buffer, computed RGB)
is correct if you inspect it directly. The right half worked, because its
own `led_max` does legitimately climb to 60 over a few iterations.

Every other built-in effect (`SOLID_COLOR` etc.) gets this right by calling
the split-aware helper instead:

```c
return rgb_matrix_check_finished_leds(led_max);
```

**Fix:** swap that one line. See `patches/vialrgb_direct_anim.h` for the
complete corrected file — apply it to the cloned QMK tree, it can't live
inside `keymaps/vial/` since the buggy file is a quantum-level animation,
not a keyboard/keymap file.

**Porting checklist:** if you're adding a custom RGB Matrix effect (Direct
mode or otherwise) to a split keyboard, grep the effect body for any
`return`/finish condition and make sure it's split-aware. If it hardcodes
`RGB_MATRIX_LED_COUNT` instead of calling `rgb_matrix_check_finished_leds()`,
it will silently never flush on whichever half doesn't own the top of the
LED index range.

### 2b. Layer changes never reach the slave half

**File:** this keymap's `keymap.c`, `layer_state_set_user`

QMK split keyboards resolve layer state (and run all `layer_state_set_*`
hooks) **only on the master**. The slave's job is limited to scanning its
own matrix and forwarding raw key events over the split serial link — it
never independently computes what layer is active. So a naive
`layer_state_set_user` that just updates a local `rgb_active_layer` variable
and repaints `g_direct_mode_colors` works perfectly on the master and does
*nothing* on the slave: its `rgb_active_layer` stays at its boot-time value
forever, and its LEDs stay stuck showing whichever layer was active at boot
(BASE, in this case), no matter what layer you actually switch to.

**Fix:** push the new active layer to the slave explicitly, the same way
color data is pushed:

```c
layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer  = get_highest_layer(state);
    rgb_active_layer = (layer < RGB_LAYER_COUNT) ? layer : BASE;
    apply_active_layer_to_direct_buffer();

    if (is_keyboard_master()) {
        transaction_rpc_send(RGB_LAYER_ACTIVE_SYNC, sizeof(rgb_active_layer), &rgb_active_layer);
    }
    return state;
}
```

with a matching slave-side RPC handler that sets `rgb_active_layer` and calls
`apply_active_layer_to_direct_buffer()` locally. See `RGB_LAYER_ACTIVE_SYNC`
in `keymap.c` for the full implementation.

**Porting checklist:** any keymap-level hook that only naturally fires on
the master (`layer_state_set_user`, `default_layer_set_user`,
`led_update_user` for host LED state, etc.) needs its own explicit RPC push
if a slave-side feature (RGB, an OLED, anything visual) depends on that
state. QMK's built-in `RGB_MATRIX_SPLIT` transport sync only mirrors
`rgb_matrix_config` (mode/hue/sat/val/speed/enable) — it does not know about
your own keymap-level state like "which of my custom color layers is active."

### 2c. Calling the LED driver synchronously from inside an RPC handler was unreliable

While diagnosing the above, an early version of a slave-side test handler
called `rgb_matrix_set_color_all()` + `rgb_matrix_update_pwm_buffers()`
**directly inside the RPC callback** (i.e. from within split-transport
transaction-handling context). This didn't reliably light the LEDs, even
though the exact same calls worked fine when made from `raw_hid_receive_kb`
on the master (a different execution context — USB HID processing, not
split serial transaction handling).

**Fix / pattern to follow:** RPC handlers on the slave should only ever
**update state** (write to `g_direct_mode_colors`, `rgb_layer_colors`,
`rgb_matrix_config` fields, etc.) and let the slave's own independent
`rgb_matrix_task()` — which keeps running continuously regardless of RPC
activity, roughly every `RGB_MATRIX_LED_FLUSH_LIMIT` (16ms default) — pick
up the change on its next natural cycle and render/flush it normally. Don't
call driver functions (`rgb_matrix_set_color*`, `rgb_matrix_update_pwm_buffers`,
anything touching `rgb_matrix_driver`) synchronously from inside an RPC
handler. This is exactly why `rgb_direct_sync_slave_handler` and
`rgb_layer_active_sync_slave_handler` in this keymap only ever do a `memcpy`
+ state update, never a direct driver call.

## 3. Diagnostic tooling worth keeping (or re-adding) when porting

These proved essential and are cheap to re-add if you strip them out for a
"clean" release keymap:

- **`RGB_DEBUG_CMD` (`0xF0`)** — reports the *local* half's `rgb_matrix`
  state (enabled, raw enable flag, active mode, expected mode, suspend
  state, master/slave role, active layer) plus a caller-supplied probe LED's
  buffer contents and the actual HSV→RGB conversion the firmware would push
  to the driver. Critically: **make the probe LED index a request parameter**
  (`data[1]` on the incoming report), not hardcoded — an earlier version
  hardcoded LED 0/30 and produced a false "nothing changed" reading simply
  because the test happened to paint a different LED.
- **`RGB_STATE_QUERY`** — a *round-trip* RPC (`transaction_rpc_exec`, not
  `transaction_rpc_send`) that lets the master ask the slave for the exact
  same state snapshot. `raw_hid_receive_kb` only ever fires on whichever
  half is USB-connected, so without this there is no way to inspect the
  other half's live RAM state at all — you're reduced to guessing from
  visual symptoms alone.
- **A main-loop tick counter** (`housekeeping_task_user`, exposed via
  `RGB_STATE_QUERY`) — proves the slave's own task loop is alive and ticking
  independent of anything RGB/USB-related, ruling out "the whole slave MCU
  is stuck" as an explanation before chasing RGB-specific theories.
- **A "bypass everything, force a raw color + immediate flush" command**
  (`RGB_RAW_BYPASS_CMD`, pushed to the slave via `RGB_RAW_BYPASS_SYNC`) —
  isolates "is the LED driver itself capable of showing color on this half"
  from every layer of effect/mode/task-state-machine logic sitting on top of
  it. When this and `RGB_FORCE_SOLID_CMD` (switch to plain `SOLID_COLOR`)
  both work identically on both halves but Direct mode doesn't, that's the
  strongest possible signal the bug is specific to the Direct-mode effect
  function, not the driver.

All four are implemented but hidden from the UI (`style="display:none"` on
their buttons in `vial-rgb-editor_1.html`) rather than deleted, precisely so
a future debugging session doesn't have to reinvent them from scratch.

## 4. Porting checklist (different split keyboard, same VialRGB approach)

1. Confirm `VIALRGB_ENABLE` and `RGB_MATRIX_ENABLE` are on, and check whether
   `RGB_MATRIX_SPLIT` is defined for your board (search the generated
   `.build/obj_.../src/info_config.h` after a build, or `keyboard.json`'s
   `rgb_matrix.split_count`) — if your board has RGB LEDs on both halves and
   this isn't set, per-key split rendering won't work correctly regardless of
   anything else here.
2. Apply the `vialrgb_direct_anim.h` fix (§2a) — check your QMK fork/version
   for the same hardcoded-vs-split-aware finish-check bug; it may already be
   fixed upstream in newer QMK trees.
3. Re-derive the row/col → LED-index mapping from **your** board's
   `keyboard.json` (`rgb_matrix.layout`, ordered list, array index = LED
   index) — don't reuse this board's mapping, LED wiring order is
   board-specific.
4. Re-check `RGB_MATRIX_LED_COUNT`, `EECONFIG_USER_DATA_SIZE` (must equal
   `3 layers × your LED count × 3 bytes`), and `RGB_MATRIX_MAXIMUM_BRIGHTNESS`
   against your board's actual values.
5. Keep `layer_state_set_user`'s explicit slave push (§2b) — this isn't
   Sofle-specific, it's inherent to how QMK splits work.
6. If you add any other slave-visible state, follow the "state-only RPC
   handler" pattern (§2c), never call driver functions from inside one.
7. Update the byte-layout constants in `vial-rgb-editor_1.html`
   (`CMD_RGB_LAYER_GET`/`SET`, `RGB_MAX_V`, `RGB_LAYER_COUNT`/`LAYER_NAMES`)
   to match your keymap — that file is the protocol spec, keep both ends in
   sync as documented in `README.md`.
