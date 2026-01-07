/* Copyright 2026 Cipulot
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

#include "hybrid_switch_matrix.h"
#include "keyboard.h"

// Definition of SOCD shared instance
socd_cleaner_t socd_opposing_pairs[4];

void eeconfig_init_kb(void) {
    // Initialize all keys with default values
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            key_state_t *key                = &eeprom_ec_config.eeprom_key_state[row][col];
            key->switch_type                = DEFAULT_SWITCH_TYPE;
            key->actuation_mode             = DEFAULT_ACTUATION_MODE;
            key->apc_actuation_threshold    = DEFAULT_APC_ACTUATION_LEVEL;
            key->apc_release_threshold      = DEFAULT_APC_RELEASE_LEVEL;
            key->rt_initial_deadzone_offset = DEFAULT_RT_INITIAL_DEADZONE_OFFSET;
            key->rt_actuation_offset        = DEFAULT_RT_ACTUATION_OFFSET;
            key->rt_release_offset          = DEFAULT_RT_RELEASE_OFFSET;
            key->bottoming_calibration_reading          = DEFAULT_BOTTOMING_CALIBRATION_READING;
        }
    }

    // Initialize the SOCD cleaner pairs
    const struct {
        uint16_t key1;
        uint16_t key2;
    } socd_pairs[] = {
        {KC_A, KC_D},
        {KC_W, KC_S},
        {KC_Z, KC_X},
        {KC_LEFT, KC_RIGHT},
    };

    for (int i = 0; i < 4; i++) {
        eeprom_ec_config.socd_opposing_pairs[i].keys[0]    = socd_pairs[i].key1;
        eeprom_ec_config.socd_opposing_pairs[i].keys[1]    = socd_pairs[i].key2;
        eeprom_ec_config.socd_opposing_pairs[i].resolution = SOCD_CLEANER_OFF;
        eeprom_ec_config.socd_opposing_pairs[i].held[0]    = false;
        eeprom_ec_config.socd_opposing_pairs[i].held[1]    = false;
    }

    // Write default value to EEPROM now
    eeconfig_update_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);

    eeconfig_init_user();
}

// On Keyboard startup
void keyboard_post_init_kb(void) {
    // Read custom menu variables from memory
    eeconfig_read_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);

    ec_config.bottoming_calibration = false;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            key_state_t *key_eeprom  = &eeprom_ec_config.eeprom_key_state[row][col];
            key_state_t *key_runtime = &ec_config.key_state[row][col];

            // Copy from EEPROM to runtime
            *key_runtime = *key_eeprom;

            // Initialize runtime-only values (noise_floor already set by ec_noise_floor())
            key_runtime->bottoming_calibration_starter = true;
            rescale_key_thresholds(key_runtime);
        }
    }

    memcpy(socd_opposing_pairs, eeprom_ec_config.socd_opposing_pairs, sizeof(socd_opposing_pairs));

    keyboard_post_init_user();
}
