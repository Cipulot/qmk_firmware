/* Copyright 2023 Cipulot
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

#include <stdint.h>
#include <stdbool.h>
#include "matrix.h"
#include "eeconfig.h"
#include "util.h"
#include "socd_cleaner.h"

typedef struct {
    uint8_t  base_actuation_mode;                // 0: APC, 1: Rapid Trigger
    uint16_t base_mode_0_actuation_threshold;    // per-key base thresholds
    uint16_t base_mode_0_release_threshold;
    uint16_t base_mode_1_initial_deadzone_offset;
    uint8_t  base_mode_1_actuation_offset;
    uint8_t  base_mode_1_release_offset;

    uint16_t noise_floor;                        // detected noise floor
    uint16_t extremum;                           // rapid-trigger extremum tracker
    bool     bottoming_calibration_starter;      // pending bottoming sample flag
    uint16_t bottoming_reading;                  // per-key bottoming reading

    uint16_t rescaled_mode_0_actuation_threshold;     // rescaled thresholds using noise_floor/bottoming
    uint16_t rescaled_mode_0_release_threshold;
    uint16_t rescaled_mode_1_initial_deadzone_offset;
    uint8_t  rescaled_mode_1_actuation_offset;
    uint8_t  rescaled_mode_1_release_offset;
} key_state_t;

typedef struct PACKED {
    uint8_t        actuation_mode;                                               // board-wide default mode
    uint8_t        base_actuation_mode[MATRIX_ROWS][MATRIX_COLS];                // per-key base mode (0xFF inherit)
    uint16_t       base_mode_0_actuation_threshold[MATRIX_ROWS][MATRIX_COLS];    // per-key base thresholds
    uint16_t       base_mode_0_release_threshold[MATRIX_ROWS][MATRIX_COLS];
    uint16_t       base_mode_1_initial_deadzone_offset[MATRIX_ROWS][MATRIX_COLS];
    uint8_t        base_mode_1_actuation_offset[MATRIX_ROWS][MATRIX_COLS];
    uint8_t        base_mode_1_release_offset[MATRIX_ROWS][MATRIX_COLS];
    uint16_t       bottoming_reading[MATRIX_ROWS][MATRIX_COLS];                  // per-key bottoming reading
    socd_cleaner_t socd_opposing_pairs[4];                                       // SOCD
} eeprom_ec_config_t;

typedef struct {
    uint8_t     actuation_mode;   // board-wide default mode
    bool        bottoming_calibration;
    key_state_t key_state[MATRIX_ROWS][MATRIX_COLS];
} ec_config_t;

// Check if the size of the reserved persistent memory is the same as the size of struct eeprom_ec_config_t
_Static_assert(sizeof(eeprom_ec_config_t) == EECONFIG_KB_DATA_SIZE, "Mismatch in keyboard EECONFIG stored data");

extern eeprom_ec_config_t eeprom_ec_config;

extern ec_config_t ec_config;

extern socd_cleaner_t socd_opposing_pairs[4];

void init_row(void);
void init_amux(void);
void disable_unused_row(uint8_t row);
void select_amux_channel(uint8_t channel, uint8_t col);
void disable_unused_amux(uint8_t channel);
void discharge_capacitor(void);
void charge_capacitor(uint8_t row);

int      ec_init(void);
void     ec_noise_floor(void);
bool     ec_matrix_scan(matrix_row_t current_matrix[]);
uint16_t ec_readkey_raw(uint8_t channel, uint8_t row, uint8_t col);
bool     ec_update_key(matrix_row_t *current_row, uint8_t row, uint8_t col, uint16_t sw_value);
void     ec_print_matrix(void);
void     rescale_key_thresholds(key_state_t *key);

uint16_t rescale(uint16_t x, uint16_t out_min, uint16_t out_max);

#ifdef UNUSED_POSITIONS_LIST
bool is_unused_position(uint8_t row, uint8_t col);
#endif

#ifdef SPLIT_KEYBOARD
void via_cmd_slave_handler(uint8_t m2s_size, const void *m2s_buffer, uint8_t s2m_size, void *s2m_buffer);
#endif
