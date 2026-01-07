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

#ifdef QUANTUM_PAINTER_ENABLE
#    include "graphics/display.h"
#endif

#ifdef SPLIT_KEYBOARD
#    include "transactions.h"
#endif

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

#ifdef SPLIT_KEYBOARD
    transaction_register_rpc(RPC_ID_VIA_CMD, via_cmd_slave_handler);
    transaction_register_rpc(RPC_ID_KB_EEPROM_SYNC, kb_eeprom_cmd_slave_handler);

    if (is_keyboard_master()) {
        // Sync EEPROM config to slave side
        // This is done to ensure both sides have the same configuration in the case one side (either the master or the slave) is connected by itself and has its own EEPROM data.
        // transaction_rpc_send(RPC_ID_KB_EEPROM_SYNC, sizeof(eeprom_ec_config_t), &eeprom_ec_config);
        uint16_t config_size = sizeof(eeprom_ec_config_t);
        for (uint16_t offset = 0; offset < config_size; offset += 255) {
            uint8_t chunk_size = (config_size - offset > 255) ? 255 : (config_size - offset);
            transaction_rpc_send(RPC_ID_KB_EEPROM_SYNC, chunk_size, (uint8_t *)&eeprom_ec_config + offset);
        }
    }
#endif

#ifdef QUANTUM_PAINTER_ENABLE
    display_init_kb();
#endif

    keyboard_post_init_user();
}

#ifdef SPLIT_KEYBOARD
/*
void kb_eeprom_cmd_slave_handler(uint8_t m2s_size, const void *m2s_buffer, uint8_t s2m_size, void *s2m_buffer) {
    if (m2s_size == sizeof(eeprom_ec_config_t)) {
        memcpy(&eeprom_ec_config, m2s_buffer, sizeof(eeprom_ec_config_t));
        eeconfig_update_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);
    } else {
        uprintf("Unexpected response in slave handler of EEPROM sync\n");
    }
}
*/
static uint16_t eeprom_sync_offset = 0;

void kb_eeprom_cmd_slave_handler(uint8_t m2s_size, const void *m2s_buffer, uint8_t s2m_size, void *s2m_buffer) {
    if (m2s_size > 0) {
        // Copy chunk into the appropriate offset
        memcpy((uint8_t *)&eeprom_ec_config + eeprom_sync_offset, m2s_buffer, m2s_size);
        eeprom_sync_offset += m2s_size;

        // Check if we've received all data
        if (eeprom_sync_offset >= sizeof(eeprom_ec_config_t)) {
            eeconfig_update_kb_datablock(&eeprom_ec_config, 0, EECONFIG_KB_DATA_SIZE);
            eeprom_sync_offset = 0; // Reset for next sync
        }
    } else {
        uprintf("Unexpected response in slave handler of EEPROM sync\n");
    }
}
#endif
