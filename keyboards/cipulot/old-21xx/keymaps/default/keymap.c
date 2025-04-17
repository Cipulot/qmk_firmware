/* Copyright 2023 Cipulot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H

enum my_keycodes { KEYPRESS = SAFE_RANGE };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // clang-format off
    /* Default */
    [0] = LAYOUT(
		                            KC_F13,  KC_F14,  KC_F15,  KC_F16,  KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
                                    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  LT(3, KC_F12),
        HPT_TOG,  KC_ESC,  KC_GRV,   KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_JYEN, KC_BSPC,  KC_INS,  KC_HOME, KC_PGUP,
        KEYPRESS, KC_PSCR, KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS,           KC_DEL,  KC_END,  KC_PGDN,
        KC_NUM,   KC_DEL,  KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_ENT,   KC_COPY, KC_UP,   KC_PASTE,
        MO(1),    KC_PAUS, KC_LSFT,  KC_BSLS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT, KC_RGUI,           KC_LEFT, KC_HOME, KC_RIGHT,
        TG(2),    KC_LGUI, KC_LCTRL, KC_LGUI, KC_LALT,                                     KC_SPC,                                      KC_RALT, KC_RGUI, KC_RCTRL, KC_CUT,  KC_DOWN, KC_APP),

    /* Mouse & Arrow Layer */
    [1] = LAYOUT(
		                            _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        HPT_FBK,  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        HPT_DWLI, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______, _______, _______, _______, _______, KC_BTN1, KC_MS_U, KC_BTN2,
        HPT_DWLD, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          KC_MS_L, _______, KC_MS_R,
        _______,  _______, _______, _______, _______,                                     _______,                                     _______, _______, _______, _______, KC_MS_D, _______),

    /* Num Layer */
    [2] = LAYOUT(
		                           _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P7,   KC_P8,   KC_P9,   KC_PEQL, KC_PMNS, KC_PPLS, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P4,   KC_P5,   KC_P6,   _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P1,   KC_P2,   KC_P3,   _______, KC_PAST, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_P0,   _______, KC_PDOT, KC_PSLS, _______, _______,          _______, _______, _______,
        _______, _______, _______, _______, _______,                                     _______,                                     _______, _______, _______, _______, _______, _______),

    /* Macro Layer */
    [3] = LAYOUT(
		                           _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______,
        _______, _______, _______, _______, _______,                                     _______,                                     _______, _______, _______, _______, _______, _______)
    // clang-format on
};

typedef union {
    uint32_t raw;
    struct {
        bool keypress_led_enabled : 1;
    };
} user_config_t;

user_config_t user_config;

/* EEPROM is getting reset */
void eeconfig_init_user(void) {
    user_config.keypress_led_enabled = true; // enable keypress LED by default
    eeconfig_update_user(user_config.raw);    // write the new default config to EEPROM
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user(); // read the user config from EEPROM
}

layer_state_t layer_state_set_user(layer_state_t state) {
    if (biton32(state) == 1) {
        gpio_write_pin_high(LAYER_INDICATOR_LED_1);
        gpio_write_pin_low(LAYER_INDICATOR_LED_2);
        gpio_write_pin_low(LAYER_INDICATOR_LED_3);
    } else if (biton32(state) == 2) {
        gpio_write_pin_high(LAYER_INDICATOR_LED_2);
        gpio_write_pin_low(LAYER_INDICATOR_LED_1);
        gpio_write_pin_low(LAYER_INDICATOR_LED_3);
    } else if (biton32(state) == 3) {
        gpio_write_pin_high(LAYER_INDICATOR_LED_3);
        gpio_write_pin_low(LAYER_INDICATOR_LED_1);
        gpio_write_pin_low(LAYER_INDICATOR_LED_2);
    }
    return state;
}

void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (keycode == KEYPRESS) {
        if (record->event.pressed) {
            user_config.keypress_led_enabled ^= 1; // Toggles the status
            eeconfig_update_user(user_config.raw); // Saves the new status
        }
    }
    if (user_config.keypress_led_enabled) {
        if (record->event.pressed) {
            gpio_write_pin_high(KEYPRESS_LED_PIN); // Turns on the LED
        } else {
            gpio_write_pin_low(KEYPRESS_LED_PIN); // Turns off the LED
        }
    } else {
        gpio_write_pin_low(KEYPRESS_LED_PIN); // Turns off the LED just to be sure
    }
}
