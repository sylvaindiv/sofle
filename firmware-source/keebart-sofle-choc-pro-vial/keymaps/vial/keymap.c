// Copyright 2023 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "transactions.h"
#include <string.h>

enum layers {
    BASE,  // default layer
    LOWER, // lower layer
    RAISE  // raise layer
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  `   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | ESC  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  | Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|  MUTE |    | PAUSE |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */

[BASE] = LAYOUT_split_4x6_5(
  KC_GRV,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_GRV,
  KC_ESC,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_BSPC,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,    KC_MPLY,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                 KC_LGUI,KC_LALT,KC_LCTL, TL_LOWR, KC_ENT,      KC_SPC,  TL_UPPR, KC_RCTL, KC_RALT, KC_RGUI
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |      |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |  `   |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   !  |   @  |   #  |   $  |   %  |-------.    ,-------|   ^  |   &  |   *  |   (  |   )  |   |  |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * | Shift|  =   |  -   |  +   |   {  |   }  |-------|    |-------|   [  |   ]  |   ;  |   :  |   \  | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[LOWER] = LAYOUT_split_4x6_5(
  _______,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,
  KC_GRV,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                       KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_F12,
  _______, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                       KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_PIPE,
  _______,  KC_EQL, KC_MINS, KC_PLUS, KC_LCBR, KC_RCBR, _______,       _______, KC_LBRC, KC_RBRC, KC_SCLN, KC_COLN, KC_BSLS, _______,
                       _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
/* RAISE
 * ,----------------------------------------.                    ,-----------------------------------------.
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Esc  | Ins  | Pscr | Menu |      |      |                    |      |      |  Up  |      | DLine| Bspc |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Rigth|  Del | Bspc |
 * |------+------+------+------+------+------|        |    |       |------+------+------+------+------+------|
 * |Shift | Undo |  Cut | Copy | Paste|      |-------|    |-------|      |      |      |      |      | Shift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LGUI | LAlt | LCTR |LOWER | /Enter  /       \Space \  |RAISE | RCTR | RAlt | RGUI |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'           '------''---------------------------'
 */
[RAISE] = LAYOUT_split_4x6_5(
  _______, _______ , _______ , _______ , _______ , _______,                           _______,  _______  , _______,  _______ ,  _______ ,_______,
  _______,  KC_INS,  KC_PSCR,   KC_APP,  XXXXXXX, XXXXXXX,                        KC_PGUP, XXXXXXX,   KC_UP, XXXXXXX,C(KC_BSPC), KC_BSPC,
  _______, KC_LALT,  KC_LCTL,  KC_LSFT,  XXXXXXX, KC_CAPS,                       KC_PGDN,  KC_LEFT, KC_DOWN, KC_RGHT,  KC_DEL, KC_BSPC,
  _______, C(KC_Z), C(KC_X), C(KC_C), C(KC_V), XXXXXXX,  _______,       _______,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   XXXXXXX, _______,
                         _______, _______, _______, _______, _______,       _______, _______, _______, _______, _______
),
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [BASE] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_MPRV, KC_MNXT) },
    [LOWER] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [RAISE] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) }
};
#endif

// =============================================================================
// Custom per-key, per-layer RGB persistence
// =============================================================================
// Reconstruction of a previously-lost firmware extension. VialRGB itself
// (VIALRGB_ENABLE, CMD_VIA_LIGHTING_*, direct-mode fastset) is stock — this
// section only adds: a color table per layer, EEPROM persistence for it, and
// split-half sync, driven by a raw HID sub-protocol whose exact byte layout
// is fixed by the reference WebHID client (vial-rgb-editor_1.html) and MUST
// NOT be changed here without changing it there too.

#define RGB_LAYER_COUNT 3 // BASE, LOWER, RAISE — must match `enum layers` above

#define CMD_RGB_LAYER_GET     0xF3
#define CMD_RGB_LAYER_SET     0xF4
#define RGB_EEPROM_SAVE_CMD   0xF1
#define RGB_DEBUG_CMD         0xF0
#define RGB_FORCE_SOLID_CMD   0xF2 // diagnostic only: bypass VialRGB/Direct entirely
#define RGB_RAW_BYPASS_CMD    0xF5 // diagnostic only: bypass the effect system entirely

extern HSV g_direct_mode_colors[RGB_MATRIX_LED_COUNT]; // VialRGB direct-mode framebuffer, quantum/vialrgb.c

static HSV    rgb_layer_colors[RGB_LAYER_COUNT][RGB_MATRIX_LED_COUNT];
static uint8_t rgb_active_layer = BASE;

_Static_assert(sizeof(rgb_layer_colors) == EECONFIG_USER_DATA_SIZE, "rgb_layer_colors size must match EECONFIG_USER_DATA_SIZE");

// Uniform per-layer factory fallback, used only on first boot (or if the
// EEPROM datablock is invalid) before anyone has touched the editor.
static const HSV rgb_layer_defaults[RGB_LAYER_COUNT] PROGMEM = {
    [BASE]  = {0, 0, 150},     // white
    [LOWER] = {170, 255, 150}, // blue
    [RAISE] = {21, 255, 150},  // orange
};

static void apply_rgb_defaults(void) {
    for (uint8_t layer = 0; layer < RGB_LAYER_COUNT; layer++) {
        HSV def;
        memcpy_P(&def, &rgb_layer_defaults[layer], sizeof(HSV));
        for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; i++) {
            rgb_layer_colors[layer][i] = def;
        }
    }
}

// Paints the currently-active layer's saved colors into the live VialRGB
// direct-mode buffer, so the physical LEDs track per-layer colors instead of
// only whatever the editor last pushed.
static void apply_active_layer_to_direct_buffer(void) {
    memcpy(g_direct_mode_colors, rgb_layer_colors[rgb_active_layer], sizeof(g_direct_mode_colors));
}

// --- Split sync ---
// Only the USB-connected half receives raw HID reports, so RAM (and, on
// save, EEPROM) updates need to be pushed to the other half explicitly.
// QMK's built-in RGB_MATRIX_SPLIT sync only covers rgb_matrix mode/hsv/speed,
// never the per-LED VialRGB direct-mode buffer — hence this custom RPC.
// One layer per call: the RPC transfer size is a uint8_t (max 255 bytes),
// and a full 3-layer table (540 bytes) doesn't fit; one layer (180 bytes) does.
typedef struct __attribute__((packed)) {
    bool    persist; // when true, the slave also writes its own EEPROM
    uint8_t layer;
    HSV     colors[RGB_MATRIX_LED_COUNT];
} rgb_sync_payload_t;

void rgb_direct_sync_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const rgb_sync_payload_t *payload = (const rgb_sync_payload_t *)in_data;
    if (payload->layer >= RGB_LAYER_COUNT) return;
    memcpy(rgb_layer_colors[payload->layer], payload->colors, sizeof(payload->colors));
    if (payload->persist) {
        eeconfig_update_user_datablock(rgb_layer_colors[payload->layer], payload->layer * sizeof(rgb_layer_colors[0]), sizeof(rgb_layer_colors[0]));
    }
    if (payload->layer == rgb_active_layer) {
        apply_active_layer_to_direct_buffer();
    }
}

static void rgb_layer_colors_push_to_slave(uint8_t layer, bool persist) {
    if (!is_keyboard_master()) return;
    rgb_sync_payload_t payload;
    payload.persist = persist;
    payload.layer   = layer;
    memcpy(payload.colors, rgb_layer_colors[layer], sizeof(payload.colors));
    // ponytail: one retry, best-effort — split serial is occasionally lossy,
    // but a missed push just means the other half keeps its previous colors
    // until the next Apply/Save, not a functional break.
    if (!transaction_rpc_send(RGB_DIRECT_SYNC, sizeof(payload), &payload)) {
        transaction_rpc_send(RGB_DIRECT_SYNC, sizeof(payload), &payload);
    }
}

static void rgb_layer_colors_push_all_to_slave(bool persist) {
    for (uint8_t layer = 0; layer < RGB_LAYER_COUNT; layer++) {
        rgb_layer_colors_push_to_slave(layer, persist);
    }
}

// Diagnostic only: tells the slave to run the exact same driver-bypass test
// as RGB_RAW_BYPASS_CMD, locally on its own side. Needed because the master
// alone has no way to make the slave's LEDs do anything otherwise.
void rgb_raw_bypass_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    rgb_matrix_set_color_all(RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0, 0);
    rgb_matrix_update_pwm_buffers();
}

// --- Remote diagnostic query ---
// raw_hid_receive_kb only ever fires on the USB-connected half, so there is
// no other way to inspect the other half's live RAM state. This round-trip
// RPC lets the master ask the slave to report its own rgb_matrix_config /
// direct-mode buffer, for the "does the slave actually think it's in Direct
// mode?" question that visual inspection alone can't answer.
typedef struct __attribute__((packed)) {
    uint8_t probe;
} rgb_state_query_req_t;

typedef struct __attribute__((packed)) {
    uint8_t enabled;
    uint8_t enable_raw;
    uint8_t mode;
    uint8_t suspended;
    uint8_t is_master;
    uint8_t active_layer;
    HSV     direct;
} rgb_state_query_resp_t;

void rgb_state_query_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const rgb_state_query_req_t *req  = (const rgb_state_query_req_t *)in_data;
    rgb_state_query_resp_t      *resp = (rgb_state_query_resp_t *)out_data;
    uint8_t                      probe = (req->probe < RGB_MATRIX_LED_COUNT) ? req->probe : 0;

    resp->enabled      = rgb_matrix_is_enabled() ? 1 : 0;
    resp->enable_raw   = rgb_matrix_config.enable;
    resp->mode         = rgb_matrix_get_mode();
    resp->suspended    = rgb_matrix_get_suspend_state() ? 1 : 0;
    resp->is_master    = is_keyboard_master() ? 1 : 0;
    resp->active_layer = rgb_active_layer;
    resp->direct       = g_direct_mode_colors[probe];
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RGB_DIRECT_SYNC, rgb_direct_sync_slave_handler);
    transaction_register_rpc(RGB_STATE_QUERY, rgb_state_query_slave_handler);
    transaction_register_rpc(RGB_RAW_BYPASS_SYNC, rgb_raw_bypass_slave_handler);

    if (eeconfig_is_user_datablock_valid()) {
        eeconfig_read_user_datablock(rgb_layer_colors, 0, sizeof(rgb_layer_colors));
    } else {
        apply_rgb_defaults();
        eeconfig_update_user_datablock(rgb_layer_colors, 0, sizeof(rgb_layer_colors));
    }
    apply_active_layer_to_direct_buffer();
    rgb_matrix_enable_noeeprom();
    rgb_matrix_mode_noeeprom(RGB_MATRIX_VIALRGB_DIRECT);
}

layer_state_t layer_state_set_user(layer_state_t state) {
    uint8_t layer  = get_highest_layer(state);
    rgb_active_layer = (layer < RGB_LAYER_COUNT) ? layer : BASE;
    apply_active_layer_to_direct_buffer();
    return state;
}

void raw_hid_receive_kb(uint8_t *data, uint8_t length) {
    switch (data[0]) {
        case RGB_EEPROM_SAVE_CMD: {
            eeconfig_update_user_datablock(rgb_layer_colors, 0, sizeof(rgb_layer_colors));
            rgb_layer_colors_push_all_to_slave(true);
            data[1] = 1; // ack
            break;
        }

        // Diagnostic only: switches to plain RGB_MATRIX_SOLID_COLOR (no
        // VialRGB, no Direct mode, no per-key data at all) at max brightness,
        // to tell apart "the LED driver/task pipeline is broken" from "only
        // our Direct-mode path is broken".
        case RGB_FORCE_SOLID_CMD: {
            rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
            rgb_matrix_sethsv_noeeprom(0, 255, RGB_MATRIX_MAXIMUM_BRIGHTNESS);
            rgb_matrix_enable_noeeprom();
            data[1] = rgb_matrix_get_mode();
            data[2] = rgb_matrix_is_enabled() ? 1 : 0;
            break;
        }

        // Diagnostic only: bypasses the whole effect/mode/task-state-machine
        // system entirely — writes straight to the LED driver and forces an
        // immediate flush. If this doesn't light up either, the bug is below
        // the effect system (driver/task); if it does, the bug is specific
        // to how the task state machine reaches FLUSHING for mode 45.
        case RGB_RAW_BYPASS_CMD: {
            rgb_matrix_set_color_all(RGB_MATRIX_MAXIMUM_BRIGHTNESS, 0, 0);
            rgb_matrix_update_pwm_buffers();
            bool peer_ok = transaction_rpc_send(RGB_RAW_BYPASS_SYNC, 0, NULL);
            if (!peer_ok) peer_ok = transaction_rpc_send(RGB_RAW_BYPASS_SYNC, 0, NULL);
            data[1] = 1; // ack (this half)
            data[2] = peer_ok ? 1 : 0; // whether the push to the other half succeeded
            break;
        }

        case RGB_DEBUG_CMD: {
            // Probe LED index is caller-supplied (data[1] on the request) so
            // we inspect whatever LED was actually just painted, instead of
            // a hardcoded guess.
            uint8_t probe = data[1];
            if (probe >= RGB_MATRIX_LED_COUNT) probe = 0;

            data[1]  = rgb_matrix_is_enabled() ? 1 : 0;      // runtime enable
            data[2]  = rgb_matrix_config.enable;             // raw eeprom-backed enable field (0/1/2/3, 2-bit)
            data[3]  = rgb_matrix_get_mode();                // active effect id
            data[4]  = RGB_MATRIX_VIALRGB_DIRECT;            // expected effect id for Direct
            data[5]  = rgb_matrix_get_suspend_state() ? 1 : 0;
            data[6]  = is_keyboard_master() ? 1 : 0;
            data[7]  = rgb_active_layer;                     // our own tracked "active" layer
            data[8]  = probe;                                // which LED index this report is about
            data[9]  = g_direct_mode_colors[probe].h;         // what's actually being rendered
            data[10] = g_direct_mode_colors[probe].s;
            data[11] = g_direct_mode_colors[probe].v;
            data[12] = rgb_layer_colors[rgb_active_layer][probe].h; // what's stored for the active layer in RAM
            data[13] = rgb_layer_colors[rgb_active_layer][probe].s;
            data[14] = rgb_layer_colors[rgb_active_layer][probe].v;
            RGB rgb  = hsv_to_rgb(g_direct_mode_colors[probe]);
            data[15] = rgb.r;                                 // actual HSV->RGB conversion firmware would push to the LED driver
            data[16] = rgb.g;
            data[17] = rgb.b;

            // Round-trip to the OTHER half so both sides' state show up in a
            // single report instead of only whichever half is USB-connected.
            rgb_state_query_req_t  peer_req  = {.probe = probe};
            rgb_state_query_resp_t peer_resp = {0};
            bool                   peer_ok   = transaction_rpc_exec(RGB_STATE_QUERY, sizeof(peer_req), &peer_req, sizeof(peer_resp), &peer_resp);
            data[18] = peer_ok ? 1 : 0;
            data[19] = peer_resp.enabled;
            data[20] = peer_resp.enable_raw;
            data[21] = peer_resp.mode;
            data[22] = peer_resp.suspended;
            data[23] = peer_resp.is_master;
            data[24] = peer_resp.active_layer;
            data[25] = peer_resp.direct.h;
            data[26] = peer_resp.direct.s;
            data[27] = peer_resp.direct.v;
            break;
        }

        case CMD_RGB_LAYER_GET: {
            uint8_t  layer = data[1];
            uint16_t idx   = data[2] | (data[3] << 8);
            uint8_t  count = data[4];
            if (layer >= RGB_LAYER_COUNT || idx + count > RGB_MATRIX_LED_COUNT) {
                data[0] = 0xFF;
                break;
            }
            for (uint8_t i = 0; i < count; i++) {
                data[5 + i * 3 + 0] = rgb_layer_colors[layer][idx + i].h;
                data[5 + i * 3 + 1] = rgb_layer_colors[layer][idx + i].s;
                data[5 + i * 3 + 2] = rgb_layer_colors[layer][idx + i].v;
            }
            break;
        }

        case CMD_RGB_LAYER_SET: {
            uint8_t  layer = data[1];
            uint16_t idx   = data[2] | (data[3] << 8);
            uint8_t  count = data[4];
            if (layer >= RGB_LAYER_COUNT || idx + count > RGB_MATRIX_LED_COUNT) {
                data[0] = 0xFF;
                break;
            }
            for (uint8_t i = 0; i < count; i++) {
                rgb_layer_colors[layer][idx + i].h = data[5 + i * 3 + 0];
                rgb_layer_colors[layer][idx + i].s = data[5 + i * 3 + 1];
                rgb_layer_colors[layer][idx + i].v = data[5 + i * 3 + 2];
            }
            if (layer == rgb_active_layer) {
                apply_active_layer_to_direct_buffer();
            }
            rgb_layer_colors_push_to_slave(layer, false);
            break;
        }

        default:
            data[0] = 0xFF; // id_unhandled — matches VIA's stock stub behavior
            break;
    }
}

#ifdef OLED_ENABLE
oled_rotation_t oled_init_kb(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    }
    return rotation;
}

static void render_logo(void) {
    static const char PROGMEM qmk_logo[] = {0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0};
    oled_write_P(qmk_logo, false);
}

void print_status_narrow(void) {
    // oled_write_P(PSTR("\n\n"), false);
    oled_write_P(PSTR("LAYER"), false);
    oled_write_P(PSTR("\n\n"), false);

    switch (get_highest_layer(layer_state)) {
        case BASE:
            oled_write_ln_P(PSTR("Base\n"), false);
            break;
        case LOWER:
            oled_write_ln_P(PSTR("Lower\n"), false);
            break;
        case RAISE:
            oled_write_ln_P(PSTR("Raise\n"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
            break;
    }

    oled_write_P(PSTR("\n\n"), false);

    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUM\n") : PSTR("\n"), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS\n") : PSTR("\n"), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCROLL\n") : PSTR("\n"), false);
}

bool oled_task_kb(void) {
    if (!oled_task_user()) {
        return false;
    }
    if (is_keyboard_master()) {
        print_status_narrow();
    } else {
        render_logo();
    }
    return true;
}

#endif
