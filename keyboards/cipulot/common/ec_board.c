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

#include "ec_switch_matrix.h"
#include "keyboard.h"

#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif

// Definition of SOCD shared instance
socd_cleaner_t socd_opposing_pairs[4];

void eeconfig_init_kb(void) {
    // Default values
    eeprom_ec_config.actuation_mode = DEFAULT_ACTUATION_MODE;

    // Initialize per-key base actuation mode to inherit (0xFF)
    memset(eeprom_ec_config.base_actuation_mode, 0xFF, sizeof(eeprom_ec_config.base_actuation_mode));

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            eeprom_ec_config.base_mode_0_actuation_threshold[row][col]    = DEFAULT_MODE_0_ACTUATION_LEVEL;
            eeprom_ec_config.base_mode_0_release_threshold[row][col]      = DEFAULT_MODE_0_RELEASE_LEVEL;
            eeprom_ec_config.base_mode_1_initial_deadzone_offset[row][col] = DEFAULT_MODE_1_INITIAL_DEADZONE_OFFSET;
            eeprom_ec_config.base_mode_1_actuation_offset[row][col]       = DEFAULT_MODE_1_ACTUATION_OFFSET;
            eeprom_ec_config.base_mode_1_release_offset[row][col]         = DEFAULT_MODE_1_RELEASE_OFFSET;
            eeprom_ec_config.bottoming_reading[row][col]                  = DEFAULT_BOTTOMING_READING;
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

    // Set runtime values to EEPROM values
    ec_config.actuation_mode        = eeprom_ec_config.actuation_mode;
    ec_config.bottoming_calibration = false;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            key_state_t *key = &ec_config.key_state[row][col];
            // Copy base configuration from EEPROM
            key->base_actuation_mode             = eeprom_ec_config.base_actuation_mode[row][col];
            key->base_mode_0_actuation_threshold = eeprom_ec_config.base_mode_0_actuation_threshold[row][col];
            key->base_mode_0_release_threshold   = eeprom_ec_config.base_mode_0_release_threshold[row][col];
            key->base_mode_1_initial_deadzone_offset = eeprom_ec_config.base_mode_1_initial_deadzone_offset[row][col];
            key->base_mode_1_actuation_offset        = eeprom_ec_config.base_mode_1_actuation_offset[row][col];
            key->base_mode_1_release_offset          = eeprom_ec_config.base_mode_1_release_offset[row][col];
            key->bottoming_reading                   = eeprom_ec_config.bottoming_reading[row][col];
            // Initialize runtime values (noise_floor already set by ec_noise_floor())
            key->bottoming_calibration_starter  = true;
            rescale_key_thresholds(key);
        }
    }

#ifdef SPLIT_KEYBOARD
    transaction_register_rpc(RPC_ID_VIA_CMD, via_cmd_slave_handler);
#endif

    memcpy(socd_opposing_pairs, eeprom_ec_config.socd_opposing_pairs, sizeof(socd_opposing_pairs));

    keyboard_post_init_user();
}
