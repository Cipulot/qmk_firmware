/* Copyright 2025 Cipulot
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

#define MATRIX_ROWS 5
#define MATRIX_COLS 18

#define UNUSED_POSITIONS_LIST { {2, 14}, {2, 15}, {2, 16}, {2, 17}, {3, 15}, {3, 17}, {4, 3}, {4, 5}, {4, 7}, {4, 10}, {4, 15} }

#define MATRIX_ROW_PINS \
    { B15, A15, B13, B8, B9}

#define AMUX_COUNT 2
#define AMUX_MAX_COLS_COUNT 16

#define AMUX_EN_PINS \
    { B14, B5 }

#define AMUX_SEL_PINS \
    { B6, B7, B4, B3 }

#define AMUX_COL_CHANNELS_SIZES \
    { 2, 16 }

#define AMUX_0_COL_CHANNELS \
    { 0, 1 }

#define AMUX_1_COL_CHANNELS \
    { 7, 6, 5, 4, 3, 2, 1, 0, 8, 15, 14, 12, 11, 13, 9, 10}

#define AMUX_COL_CHANNELS AMUX_0_COL_CHANNELS, AMUX_1_COL_CHANNELS

#define DISCHARGE_PIN A2
#define ANALOG_PORT A3

#define DEFAULT_ACTUATION_MODE 0
#define DEFAULT_MODE_0_ACTUATION_LEVEL 550
#define DEFAULT_MODE_0_RELEASE_LEVEL 500
#define DEFAULT_MODE_1_INITIAL_DEADZONE_OFFSET DEFAULT_MODE_0_ACTUATION_LEVEL
#define DEFAULT_MODE_1_ACTUATION_OFFSET 70
#define DEFAULT_MODE_1_RELEASE_OFFSET 70
#define DEFAULT_EXTREMUM 1023
#define EXPECTED_NOISE_FLOOR 0
#define NOISE_FLOOR_THRESHOLD 50
#define BOTTOMING_CALIBRATION_THRESHOLD 50
#define DEFAULT_NOISE_FLOOR_SAMPLING_COUNT 30
#define DEFAULT_BOTTOMING_READING 1023
#define DEFAULT_CALIBRATION_STARTER true

#define DISCHARGE_TIME 10

#define EECONFIG_KB_DATA_SIZE 189
