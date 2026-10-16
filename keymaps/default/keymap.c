/*
 * blackbillKeyboard
 *
 * 104-key full-size ANSI keyboard, hand-wired on a WeAct Black Pill
 * (STM32F401).
 *
 *   Esc  F1 F2 F3 F4 F5 F6 F7 F8 F9 F10 F11 F12  PrtSc ScrLk Pause
 *   ` 1 2 3 4 5 6 7 8 9 0 - = BSpc                  Ins Home PgUp    Num / * -
 *   Tab Q W E R T Y U I O P [ ] \                    Del End PgDn    7 8 9 +
 *   Caps A S D F G H J K L ; ' Enter                                    4 5 6
 *   Shift Z X C V B N M , . / Shift                  Up                  1 2 3 Enter
 *   Ctrl Win Alt        Space          Alt Fn Menu Ctrl (arrows) Left Down Right   0 .
 */

#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _FN,
};

enum custom_keycodes {
    KC_KLOCK = SAFE_RANGE,
};

static bool keyboard_locked = false;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KC_KLOCK) {
        if (record->event.pressed) {
            keyboard_locked = !keyboard_locked;
        }
        return false;
    }
    if (keyboard_locked && record->event.pressed) {
        return keycode == MO(1);
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_fullsize_ansi(
        // clang-format off
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        KC_PSCR, KC_SCRL, KC_PAUS,

        KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,
        KC_MINS, KC_EQL,  KC_BSPC,
        KC_INS,  KC_HOME, KC_PGUP,
        KC_NUM,  KC_PSLS, KC_PAST, KC_PMNS,

        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,
        KC_LBRC, KC_RBRC, KC_BSLS,
        KC_DEL,  KC_END,  KC_PGDN,
        KC_P7,   KC_P8,   KC_P9,   KC_PPLS,

        KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,
        KC_SCLN, KC_QUOT, KC_ENT,
        KC_P4,   KC_P5,   KC_P6,

        KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,
        KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
        KC_UP,
        KC_P1,   KC_P2,   KC_P3,   KC_PENT,

        KC_LCTL, KC_LGUI, KC_LALT, KC_SPC,  KC_RALT, MO(1),   KC_APP,  KC_RCTL,
        KC_LEFT, KC_DOWN, KC_RGHT,
        KC_P0,   KC_PDOT
        // clang-format on
    ),

    [_FN] = LAYOUT_fullsize_ansi(
        // clang-format off
        QK_BOOT, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_KLOCK,
        KC_TRNS, KC_TRNS, QK_BOOT,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
        UG_TOGG, UG_NEXT, UG_PREV, UG_VALD,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
        UG_HUED, UG_HUEU, UG_SATD, UG_VALU,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS,
        UG_SATU, UG_SPDD, UG_SPDU,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_MSTP,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,

        KC_TRNS, KC_TRNS, KC_TRNS, KC_MUTE, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_MPRV, KC_MPLY, KC_MNXT,
        KC_TRNS, KC_TRNS
        // clang-format on
    ),
};