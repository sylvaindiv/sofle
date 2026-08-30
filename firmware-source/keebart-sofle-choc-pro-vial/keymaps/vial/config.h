#define VIAL_KEYBOARD_UID \
    { 0x4F, 0x2D, 0x5A, 0x8A, 0x49, 0x7C, 0xDF, 0x1D }

#define VIAL_UNLOCK_COMBO_ROWS \
    { 0, 5 }
#define VIAL_UNLOCK_COMBO_COLS \
    { 0, 0 }

#define DYNAMIC_KEYMAP_LAYER_COUNT 10

// --- Custom per-layer VialRGB persistence (see keymap.c) --------------------
// Reconstructed protocol extension; byte layout is dictated by the reference
// WebHID client (vial-rgb-editor_1.html), not by this firmware.

// Must match RGB_MAX_V in vial-rgb-editor_1.html. Overrides keyboard.json's
// max_brightness (100), hence the #undef.
#undef RGB_MATRIX_MAXIMUM_BRIGHTNESS
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 170

// 3 layers (BASE/LOWER/RAISE) * 60 LEDs * sizeof(HSV)=3 bytes.
#define EECONFIG_USER_DATA_SIZE 540

// Split sync of one layer's color table (plus an EEPROM persist flag) to the
// other half — one RPC call per layer, since the RPC transfer size field is
// a uint8_t (max 255 bytes; a full single-layer payload is 182 bytes).
// Default 32-byte RPC buffers are too small for that.
#define SPLIT_TRANSACTION_IDS_USER RGB_DIRECT_SYNC, RGB_STATE_QUERY, RGB_RAW_BYPASS_SYNC
#define RPC_M2S_BUFFER_SIZE 192
