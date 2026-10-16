#include QMK_KEYBOARD_H

static void type_position(uint8_t r, uint8_t c) {
    char buf[8];
    uint8_t i = 0;
    buf[i++] = 'R';
    buf[i++] = '0' + (r % 10);
    buf[i++] = 'C';
    if (c >= 10) {
        buf[i++] = '1';
        buf[i++] = '0' + (c - 10);
    } else {
        buf[i++] = '0' + c;
    }
    buf[i++] = ' ';
    buf[i]   = '\0';
    send_string(buf);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    (void)keycode;
    if (record->event.pressed) {
        type_position(record->event.key.row, record->event.key.col);
    }
    return false;
}

#define _21 KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A, KC_A

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = {
        { _21 },
        { _21 },
        { _21 },
        { _21 },
        { _21 },
        { _21 }
    }
};
