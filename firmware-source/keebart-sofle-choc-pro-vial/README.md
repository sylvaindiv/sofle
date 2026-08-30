# Sofle Choc Pro (Keebart) — custom Vial + per-layer RGB firmware source

This is the keymap-level source for the custom firmware whose compiled binary
lives at `../../sofle_keebart_vial.uf2` (repo root). It is a stock
[Keebart `vial-qmk-sofle-choc-pro`](https://github.com/Keebart/vial-qmk-sofle-choc-pro)
(`vial` branch) checkout, with only `keyboards/keebart/sofle_choc_pro/keymaps/vial/`
modified — everything else (VialRGB, the base keymap layout, split transport,
etc.) is unmodified upstream code.

The custom addition on top of stock VialRGB is: a color table per layer (3
layers: BASE/LOWER/RAISE), EEPROM persistence for it, and split-half RPC sync
— driven by a raw HID sub-protocol (`0xF0`/`0xF1`/`0xF3`/`0xF4`) whose exact
byte layout is defined by the reference WebHID client at
`vial-rgb-editor_1.html` (repo root). That HTML file is the protocol spec —
if you ever touch this firmware again, keep the two in sync.

## Why this file exists

The original implementation of this feature was lost (workspace deleted,
never pushed to a remote). Only the compiled `.uf2` and the WebHID client
survived. This directory is the reconstruction, rebuilt from scratch against
the same protocol, so it doesn't happen again — **commit this directory**.

## How to rebuild

```bash
# 1. Clone the upstream fork (outside this repo — it's large, ~2GB with submodules)
git clone --branch vial https://github.com/Keebart/vial-qmk-sofle-choc-pro
cd vial-qmk-sofle-choc-pro
git submodule update --init --recursive

# 2. Copy this keymap over the stock one
cp -r /path/to/khartoum/firmware-source/keebart-sofle-choc-pro-vial/keymaps/vial \
      keyboards/keebart/sofle_choc_pro/keymaps/vial

# 3. Toolchain (macOS/Homebrew) — qmk CLI + ARM/AVR cross toolchain
brew install qmk/qmk/qmk
qmk config user.qmk_home=/path/to/vial-qmk-sofle-choc-pro
export PATH="/opt/homebrew/opt/arm-none-eabi-binutils/bin:/opt/homebrew/opt/arm-none-eabi-gcc@8/bin:$PATH"

# 4. Compile
qmk compile -kb keebart/sofle_choc_pro -km vial
# -> .build/keebart_sofle_choc_pro_vial.uf2
```

Flashing (double-tap reset to enter the RP2040 UF2 bootloader, then copy the
`.uf2` onto the mounted `RPI-RP2` drive) is a manual step per keyboard half —
not covered here.

## What's implemented

- `HSV rgb_layer_colors[3][RGB_MATRIX_LED_COUNT]` — per-layer, per-LED color
  table in RAM.
- `CMD_RGB_LAYER_GET` (`0xF3`) / `CMD_RGB_LAYER_SET` (`0xF4`) — read/write that
  table directly (independent of the keyboard's currently active layer),
  batched up to 9 LEDs per HID report.
- `RGB_EEPROM_SAVE_CMD` (`0xF1`) — persists the whole table via
  `eeconfig_update_user_datablock` (`EECONFIG_USER_DATA_SIZE = 540`), and
  pushes it to the other half so both EEPROMs stay in sync.
- `RGB_DEBUG_CMD` (`0xF0`) — internal diagnostic snapshot (enabled state,
  active vs. expected direct-mode effect id, LED0/LED30 colors).
- `layer_state_set_user` — repaints `g_direct_mode_colors` from
  `rgb_layer_colors[active_layer]` whenever the active layer changes, so the
  physical LEDs track per-layer colors instead of only whatever the last HID
  write pushed.
- Split sync (`transaction_register_rpc`, transaction id `RGB_DIRECT_SYNC`) —
  QMK's built-in `RGB_MATRIX_SPLIT` transport sync only covers rgb_matrix
  mode/hsv/speed, never the per-LED VialRGB direct-mode buffer, so this
  pushes one layer's 180-byte color block (plus a persist flag) to the slave
  half per RPC call (the RPC transfer-size field is a `uint8_t`, capping any
  single call at 255 bytes — a full 3-layer, 540-byte table doesn't fit in
  one call, hence one call per layer).

## Known-uncertain / needs testing on real hardware

- **Split sync has not been tested on physical hardware** (this was rebuilt
  and compiled in an environment without the keyboard attached). Verify:
  moving between layers shows correct colors on *both* halves, not just the
  USB-connected one; after `0xF1` save + full unplug/replug, both halves
  restore the same saved colors.
- `RGB_MATRIX_MAXIMUM_BRIGHTNESS` was set to `180` in `config.h` to match
  `RGB_MAX_V` in `vial-rgb-editor_1.html` (which the client comments say
  "must match the firmware") — this overrides Keebart's stock
  `keyboard.json` value of `100`. Double check this is actually the
  brightness ceiling you want; it was inferred from the client comment, not
  verified against the original lost firmware.
- Factory-default colors (`rgb_layer_defaults` in `keymap.c`, used only on
  first boot / invalid EEPROM) are a simple guess — white/blue/orange for
  BASE/LOWER/RAISE — not a recovery of the original defaults, which are
  unknown. Change freely; they only matter before the first "Save
  permanently" from the editor.
