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

#pragma once

/* key matrix size */
#define MATRIX_ROWS 9
#define MATRIX_COLS 20

#define MATRIX_ROW_PINS \
    { A0, C3, C2, C1, B15, C6, C7, C8, C9 }
#define MATRIX_COL_PINS \
    { B5, B6, B7, B8, B9, A1, A2, A3, A4, A5, A6, A7, C4, C5, B14, B12, B13, B0, B1, B10 }

/* COL2ROW, ROW2COL */
#define DIODE_DIRECTION COL2ROW

/* Solenoid support */
#define SOLENOID_PIN A13
#define SOLENOID_DEFAULT_DWELL 15

/* Indicator LEDs */
#define LED_NUM_LOCK_PIN C11
#define LED_CAPS_LOCK_PIN C10
#define LAYER_INDICATOR_LED_1 C12
#define LAYER_INDICATOR_LED_2 B3
#define LAYER_INDICATOR_LED_3 C0
#define HAPTIC_ENABLE_STATUS_LED B4
#define KEYPRESS_LED_PIN A15

/* Debounce reduces chatter (unintended double-presses) - set 0 if debouncing is not needed */
#define DEBOUNCE 5

/* Mechanical locking support. Use KC_LCAP, KC_LNUM or KC_LSCR instead in keymap */
#define LOCKING_SUPPORT_ENABLE
/* Locking resynchronize hack */
#define LOCKING_RESYNC_ENABLE
