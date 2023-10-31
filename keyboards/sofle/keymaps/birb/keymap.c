#include QMK_KEYBOARD_H
#include "os_detection.h"
#include <stdio.h>

uint32_t keycount = 0;
int encoder_scroll_mode = 0;

enum sofle_layers {
    _QWERTY,
    _LOWER,
    _RAISE,
    _ADJUST
};

enum custom_keycodes {
    KC_QWERTY = SAFE_RANGE,
    KC_PRVWD,
    KC_NXTWD,
    KC_SCRLMD
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  BS  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | `    |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  DEL |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |  :;  |  '@  |
 * |------+------+------+------+------+------|  MUTE |    |SCROLL |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,< |   .> |  ?/  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTL | LGUI | LALT |LOWER | /Enter  /       \Space \  |RAISE | RALT | RGUI | RCTL |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */

[_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,  KC_BSPC,
  KC_GRV,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,  KC_DEL,
  KC_TAB,   KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_MUTE,  KC_SCRLMD,KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_RSFT,
                 KC_LCTL,KC_LGUI,KC_LALT, MO(_LOWER), KC_ENT,     KC_SPC,   MO(_RAISE), KC_RALT, KC_RGUI, KC_RCTL
),
/* LOWER 
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |      |      |  #~  |      |      |                    |      |      |      |  [{  |  ]}  |  BS  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |  DEL |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |      |      |      |      |      |-------.    ,-------|      |      |      |      |      | HOME |
 * |------+------+------+------+------+------|  MUTE |    |SCROLL |------+------+------+------+------+------|
 * |LShift|  \|  |  _-  |  =+  |      |      |-------|    |-------|      |      |      |      |  \|  | END  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LGUI | LALT |LOWER | /Enter  /       \Space \  |RAISE | RALT | RGUI | RCTL |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */
[_LOWER] = LAYOUT(
  _______, XXXXXXX, XXXXXXX, KC_NUHS, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, _______,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,
  _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_HOME,
  _______, KC_NUBS, KC_MINS,  KC_EQL, XXXXXXX, XXXXXXX, _______,     _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_NUBS,  KC_END,
                    _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
),
/* RAISE
 * ,----------------------------------------.                     ,-----------------------------------------.
 * | ESC  |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  | F10  | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | Ins  | Pscr | Menu |      |      |                    |      |      | PWrd | NWrd |      | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  | LAt  | LCtl |LShift|      | Caps |-------.    ,-------|      | Left | Down | Up   | Right| HOME |
 * |------+------+------+------+------+------|  MUTE |    |SCROLL |------+------+------+------+------+------|
 * |LShift| Undo |  Cut | Copy | Paste|      |-------|    |-------|      |      |      |      |      | END  |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTR | LGUI | LALT |LOWER | /Enter  /       \Space \  |RAISE | RALT | RGUI | RCTL |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */
[_RAISE] = LAYOUT(
  _______,   KC_F1,   KC_F2,   KC_F3,    KC_F4,   KC_F5,                           KC_F6,   KC_F7,    KC_F8,    KC_F9,  KC_F10,  KC_F11,
  XXXXXXX,  KC_INS, KC_PSCR,  KC_APP,  XXXXXXX, XXXXXXX,                         XXXXXXX, XXXXXXX, KC_PRVWD, KC_NXTWD, XXXXXXX,  KC_F12,
  _______, KC_LALT, KC_LCTL, KC_LSFT,  XXXXXXX, KC_CAPS,                         XXXXXXX, KC_LEFT,  KC_DOWN,    KC_UP, KC_RGHT, KC_HOME,
  _______, KC_UNDO,  KC_CUT, KC_COPY, KC_PASTE, XXXXXXX, _______,       _______, XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX, XXXXXXX, KC_END ,
                     _______, _______, _______, _______, _______,       _______, _______, _______,  _______,  _______
),
/* ADJUST
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | BOOTL|      |      |      |      |      |                    |      |      |      |      |      | BOOTL|
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |                    |      |      |      |      |      |      |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------.    ,-------|      | VOLDO| MUTE | VOLUP|      |      |
 * |------+------+------+------+------+------|  MUTE |    |SCROLL |------+------+------+------+------+------|
 * |      |      |      |      |      |      |-------|    |-------|      | PREV | PLAY | NEXT |      |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *            | LCTL | LGUI | LALT |LOWER | /Enter  /       \Space \  |RAISE | RALT | RGUI | RCTL |
 *            |      |      |      |      |/       /         \      \ |      |      |      |      |
 *            `----------------------------------'            '------''---------------------------'
 */
  [_ADJUST] = LAYOUT(
  QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                       XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, XXXXXXX, XXXXXXX,
  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, _______,     _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, XXXXXXX, XXXXXXX,
                    _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______
  )
};

#ifdef OLED_ENABLE

static void draw_leds(void) {
    led_t led_state = host_keyboard_led_state();

    if (led_state.caps_lock == true) {
        static const char PROGMEM CAPS_1[] = {
            78, 81,193, 65, 65,145, 14,  0
        };

        static const char PROGMEM CAPS_2[] = {
            20, 20, 23,244, 20, 18,241,  0
        };

        static const char PROGMEM CAPS_3[] = {
            56, 68, 64, 56,  5, 69, 56,  0
        };
        
        oled_set_cursor(18, 1);
        oled_write_raw_P(CAPS_1, sizeof(CAPS_1));

        oled_set_cursor(18, 2);
        oled_write_raw_P(CAPS_2, sizeof(CAPS_2));

        oled_set_cursor(18, 3);
        oled_write_raw_P(CAPS_3, sizeof(CAPS_3));
    }
}

static void draw_os(void) {
    switch (detected_host_os()) {
    case OS_UNSURE:
        oled_write_ln_P(PSTR("> Unknown"), false);
        break;
    case OS_LINUX:
        oled_write_ln_P(PSTR("> Linux"), false);
        break;
    case OS_WINDOWS:
        oled_write_ln_P(PSTR("> Windows"), false);
        break;
    case OS_MACOS:
        oled_write_ln_P(PSTR("> OS X"), false);
        break;
    case OS_IOS:
        oled_write_ln_P(PSTR("> iOS"), false);
        break;
    }
}

static void draw_layer(void) {
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("$ Base "), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("$ Raise"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("$ Lower"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("$ Mode "), false);
            break;
        default:
            oled_write_P(PSTR(""), false);
    }
}

static void draw_qr(void) {
    static const char PROGMEM qr1[] = {
        0xff, 0xff, 0x30, 0x30, 0xcf, 0xcf, 0x30, 0x30, 0xcf, 0xcf, 0x0c, 0x0c, 0xcf, 0xcf, 0xf0, 0xf0, 
        0x33, 0x33, 0x0c, 0x0c, 0x0f, 0x0f, 0xc0, 0xc0, 0xc3, 0xc3, 0xc0, 0xc0, 0x33, 0x33, 0xcc, 0xcc
    };
    static const char PROGMEM qr2[] = {
        0xff, 0xff, 0x3c, 0x3c, 0x3c, 0x3c, 0x03, 0x03, 0xc3, 0xc3, 0x3f, 0x3f, 0xff, 0xff, 0xf0, 0xf0, 
        0x3f, 0x3f, 0x3c, 0x3c, 0xfc, 0xfc, 0xc3, 0xc3, 0xff, 0xff, 0x30, 0x30, 0xcf, 0xcf, 0xcc, 0xcc
    };
    static const char PROGMEM qr3[] = {
        0xff, 0xff, 0xcc, 0xcc, 0xcc, 0xcc, 0x3f, 0x3f, 0x00, 0x00, 0xfc, 0xfc, 0x3f, 0x3f, 0xfc, 0xfc, 
        0x3f, 0x3f, 0xcf, 0xcf, 0xfc, 0xfc, 0xf3, 0xf3, 0xff, 0xff, 0xc3, 0xc3, 0xfc, 0xfc, 0xcc, 0xcc
    };
    static const char PROGMEM qr4[] = {
        0xff, 0xff, 0xc3, 0xc3, 0xff, 0xff, 0xcf, 0xcf, 0xcc, 0xcc, 0xf3, 0xf3, 0xfc, 0xfc, 0xc0, 0xc0, 
        0xcf, 0xcf, 0xfc, 0xfc, 0xc3, 0xc3, 0xc0, 0xc0, 0xf0, 0xf0, 0xcf, 0xcf, 0xf3, 0xf3, 0xcc, 0xcc
    };

    oled_set_cursor(16, 0);
    oled_write_raw_P(qr1, sizeof(qr1));

    oled_set_cursor(16, 1);
    oled_write_raw_P(qr2, sizeof(qr2));
    
    oled_set_cursor(16, 2);
    oled_write_raw_P(qr3, sizeof(qr3));

    oled_set_cursor(16, 3);
    oled_write_raw_P(qr4, sizeof(qr4));
}

static void draw_keycount(void) {
    oled_write_P(PSTR("count: "), false);

    char str[12];
    sprintf(str, "%ld", keycount);
    oled_write_ln(str, false);
}

static void draw_mode(void) {
    if (encoder_scroll_mode == 1) {
        oled_write_ln_P(PSTR("$ Scroll"), false);
    }
    else {
        oled_write_ln_P(PSTR("$ Page"), false);
    }

    oled_write_ln_P(PSTR(""), false);
}

static void draw_wpm(void) {
    oled_write_P(PSTR("\n\nwpm: "), false);
    long int wpm = get_current_wpm();

    char str[12];
    sprintf(str, "%ld", wpm);
    oled_write_ln(str, false);
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return rotation;
    }
    return OLED_ROTATION_180;
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        draw_os();
        draw_mode();
        draw_keycount(); 
        draw_leds();

    } else {
        oled_write_ln_P(PSTR("> miaowi.ng"), false);
        draw_layer();
        draw_wpm();
        draw_qr();
    }
    return false;
}

#endif

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) { keycount++; }

    switch (keycode) {
        case KC_QWERTY:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTY);
            }
            return false;
        case KC_SCRLMD: 
            if(record->event.pressed) {
                encoder_scroll_mode = 1 - encoder_scroll_mode;
            }
            return false;
        case KC_PRVWD:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_LEFT);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_LEFT);
            }
            return false;
        case KC_NXTWD:
             if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_RIGHT);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_RIGHT);
            }
            return false;
        case KC_COPY:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_C);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_C);
            }
            return false;
        case KC_PASTE:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_V);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_V);
            }
            return false;
        case KC_CUT:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_X);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_X);
            }
            return false;
        case KC_UNDO:
            if (record->event.pressed) {
                register_mods(mod_config(MOD_LCTL));
                register_code(KC_Z);
            } else {
                unregister_mods(mod_config(MOD_LCTL));
                unregister_code(KC_Z);
            }
            return false;
    }
    return true;
}

#ifdef ENCODER_ENABLE

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (encoder_scroll_mode == 1) { // SCROLL MODE
        if (index == 0) {
            if (!clockwise) {
                tap_code(KC_LEFT);
            } else {
                tap_code(KC_RIGHT);
            }
        } else if (index == 1) {
            if (!clockwise) {
                tap_code(KC_WH_U);
            } else {
                tap_code(KC_WH_D);
            }
        }
    }
    else { // PAGE MODE
        if (index == 0) {
            if (!clockwise) {
                tap_code(KC_UP);
            } else {
                tap_code(KC_DOWN);
            }
        } else if (index == 1) {
            if (!clockwise) {
                tap_code(KC_PGUP);
            } else {
                tap_code(KC_PGDN);
            }
        }
    }
    return false;
}

#endif
