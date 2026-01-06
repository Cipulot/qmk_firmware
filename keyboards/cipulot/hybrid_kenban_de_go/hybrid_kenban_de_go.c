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

#include "hybrid_switch_matrix.h"
#include "keyboard.h"
#include "quantum.h"

#ifdef QUANTUM_PAINTER_ENABLE
#    include "graphics/display.h"
#endif

#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif

// Definition of SOCD shared instance
socd_cleaner_t socd_opposing_pairs[4];

void eeconfig_init_kb(void) {
    // Default values
    eeprom_ec_config.switch_type                    = 0;
    eeprom_ec_config.actuation_mode                 = DEFAULT_ACTUATION_MODE;
    eeprom_ec_config.apc_actuation_threshold     = DEFAULT_APC_ACTUATION_LEVEL;
    eeprom_ec_config.apc_release_threshold       = DEFAULT_APC_RELEASE_LEVEL;
    eeprom_ec_config.rt_initial_deadzone_offset = DEFAULT_RT_INITIAL_DEADZONE_OFFSET;
    eeprom_ec_config.rt_actuation_offset        = DEFAULT_RT_ACTUATION_OFFSET;
    eeprom_ec_config.rt_release_offset          = DEFAULT_RT_RELEASE_OFFSET;

    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            eeprom_ec_config.bottoming_reading[row][col] = DEFAULT_BOTTOMING_READING;
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
    ec_config.switch_type                    = eeprom_ec_config.switch_type;
    ec_config.actuation_mode                 = eeprom_ec_config.actuation_mode;
    ec_config.apc_actuation_threshold     = eeprom_ec_config.apc_actuation_threshold;
    ec_config.apc_release_threshold       = eeprom_ec_config.apc_release_threshold;
    ec_config.rt_initial_deadzone_offset = eeprom_ec_config.rt_initial_deadzone_offset;
    ec_config.rt_actuation_offset        = eeprom_ec_config.rt_actuation_offset;
    ec_config.rt_release_offset          = eeprom_ec_config.rt_release_offset;
    ec_config.bottoming_calibration          = false;
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            ec_config.bottoming_calibration_starter[row][col]           = true;
            ec_config.bottoming_reading[row][col]                       = eeprom_ec_config.bottoming_reading[row][col];
            ec_config.rescaled_apc_actuation_threshold[row][col]     = rescale(ec_config.apc_actuation_threshold, ec_config.noise_floor[row][col], eeprom_ec_config.bottoming_reading[row][col]);
            ec_config.rescaled_apc_release_threshold[row][col]       = rescale(ec_config.apc_release_threshold, ec_config.noise_floor[row][col], eeprom_ec_config.bottoming_reading[row][col]);
            ec_config.rescaled_rt_initial_deadzone_offset[row][col] = rescale(ec_config.rt_initial_deadzone_offset, ec_config.noise_floor[row][col], eeprom_ec_config.bottoming_reading[row][col]);
            ec_config.rescaled_rt_actuation_offset[row][col]        = rescale(ec_config.rt_actuation_offset, ec_config.noise_floor[row][col], eeprom_ec_config.bottoming_reading[row][col]);
            ec_config.rescaled_rt_release_offset[row][col]          = rescale(ec_config.rt_release_offset, ec_config.noise_floor[row][col], eeprom_ec_config.bottoming_reading[row][col]);
        }
    }

    memcpy(socd_opposing_pairs, eeprom_ec_config.socd_opposing_pairs, sizeof(socd_opposing_pairs));

#ifdef SPLIT_KEYBOARD
    transaction_register_rpc(RPC_ID_VIA_CMD, via_cmd_slave_handler);
    transaction_register_rpc(RPC_ID_KB_EEPROM_SYNC, kb_eeprom_cmd_slave_handler);

    if (is_keyboard_master()) {
        // Sync EEPROM config to slave side
        // This is done to ensure both sides have the same configuration in the case one side (either the master or the slave) is connected by itself and has its own EEPROM data.
        transaction_rpc_send(RPC_ID_KB_EEPROM_SYNC, sizeof(eeprom_ec_config_t), &eeprom_ec_config);
    }
#endif

#ifdef QUANTUM_PAINTER_ENABLE
    display_init_kb();
#endif

    keyboard_post_init_user();
}

#ifdef SPLIT_KEYBOARD
void kb_eeprom_cmd_slave_handler(uint8_t m2s_size, const void *m2s_buffer, uint8_t s2m_size, void *s2m_buffer) {
    if (m2s_size == sizeof(eeprom_ec_config_t)) {
        memcpy(&eeprom_ec_config, m2s_buffer, sizeof(eeprom_ec_config_t));
        eeconfig_update_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);
    } else {
        uprintf("Unexpected response in slave handler of EEPROM sync\n");
    }
}
#endif
