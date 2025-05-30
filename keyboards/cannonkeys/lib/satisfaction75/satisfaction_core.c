// Copyright 2023 Andrew Kannan
// SPDX-License-Identifier: GPL-2.0-or-later

#include "satisfaction_core.h"
#include "print.h"
#include "debug.h"
#include "matrix.h"
#include "quantum.h"
#include "encoder.h"

#include <ch.h>
#include <hal.h>

#include "timer.h"

#include "raw_hid.h"
#include "dynamic_keymap.h"
#include "eeprom.h"
#include "version.h" // for QMK_BUILDDATE used in EEPROM magic

/* Artificial delay added to get media keys to work in the encoder*/
#define MEDIA_KEY_DELAY 10

volatile uint8_t led_numlock = false;
volatile uint8_t led_capslock = false;
volatile uint8_t led_scrolllock = false;

uint8_t layer;

bool clock_set_mode = false;
uint8_t oled_mode = OLED_DEFAULT;
bool oled_repaint_requested = false;
bool oled_wakeup_requested = false;
uint32_t oled_sleep_timer;

uint8_t encoder_value = 32;
uint8_t encoder_mode = ENC_MODE_VOLUME;
uint8_t enabled_encoder_modes = 0x1F;

RTCDateTime last_timespec;
uint16_t last_minute = 0;

uint8_t time_config_idx = 0;
int8_t hour_config = 0;
int16_t minute_config = 0;
int8_t year_config = 0;
int8_t month_config = 0;
int8_t day_config = 0;
uint8_t previous_encoder_mode = 0;

void board_init(void) {
  SYSCFG->CFGR1 |= SYSCFG_CFGR1_I2C1_DMA_RMP;
  SYSCFG->CFGR1 &= ~(SYSCFG_CFGR1_SPI2_DMA_RMP);
}

uint32_t read_custom_config(void *data, uint32_t offset, uint32_t length) {
#ifdef VIA_ENABLE
    return via_read_custom_config(data, offset, length);
#else
    return eeconfig_read_kb_datablock(data, offset, length);
#endif
}

uint32_t write_custom_config(const void *data, uint32_t offset, uint32_t length) {
#ifdef VIA_ENABLE
    return via_update_custom_config(data, offset, length);
#else
    return eeconfig_update_kb_datablock(data, offset, length);
#endif
}

void keyboard_post_init_kb(void) {
      /*
        This is a workaround to some really weird behavior
        Without this code, the OLED will turn on, but not when you initially plug the keyboard in.
        You have to manually trigger a user reset to get the OLED to initialize properly
        I'm not sure what the root cause is at this time, but this workaround fixes it.
    */
    #ifdef OLED_ENABLE
    if(!is_oled_on()){
        wait_ms(3000);
        oled_init(OLED_ROTATION_0);
    }
    #endif

    keyboard_post_init_user();
}

#ifdef VIA_ENABLE
void custom_set_value(uint8_t *data) {
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ) {
        case id_oled_default_mode:
        {
            write_custom_config(&value_data[0], EEPROM_DEFAULT_OLED_OFFSET, 1);
            break;
        }
        case id_oled_mode:
        {
            oled_mode = value_data[0];
            oled_request_wakeup();
            break;
        }
        case id_encoder_modes:
        {
            uint8_t index = value_data[0];
            uint8_t enable = value_data[1];
            enabled_encoder_modes = (enabled_encoder_modes & ~(1<<index)) | (enable<<index);
            write_custom_config(&enabled_encoder_modes, EEPROM_ENABLED_ENCODER_MODES_OFFSET, 1);
            break;
        }
        case id_encoder_custom:
        {
            uint8_t custom_encoder_idx = value_data[0];
            uint8_t encoder_behavior = value_data[1];
            uint16_t keycode = (value_data[2] << 8) | value_data[3];
            set_custom_encoder_config(custom_encoder_idx, encoder_behavior, keycode);
            break;
        }
    }
}

void custom_get_value(uint8_t *data) {
    uint8_t *value_id = &(data[0]);
    uint8_t *value_data = &(data[1]);

    switch ( *value_id ) {
        case id_oled_default_mode:
        {
            uint8_t default_oled;
            read_custom_config(&default_oled, EEPROM_DEFAULT_OLED_OFFSET, 1);
            value_data[0] = default_oled;
            break;
        }
        case id_oled_mode:
        {
            value_data[0] = oled_mode;
            break;
        }
        case id_encoder_modes:
        {
            uint8_t index = value_data[0];
            value_data[1] = (enabled_encoder_modes & (1<<index)) ? 1 : 0;
            break;
        }
        case id_encoder_custom:
        {
            uint8_t custom_encoder_idx = value_data[0];
            uint8_t encoder_behavior = value_data[1];
            uint16_t keycode = retrieve_custom_encoder_config(custom_encoder_idx, encoder_behavior);
            value_data[2] = keycode >> 8;
            value_data[3] = keycode & 0xFF;
            break;
        }
    }
}

void via_custom_value_command_kb(uint8_t *data, uint8_t length) {
    uint8_t *command_id        = &(data[0]);
    uint8_t *channel_id        = &(data[1]);
    uint8_t *value_id_and_data = &(data[2]);

    if ( *channel_id == id_custom_channel ) {
        switch ( *command_id )
        {
            case id_custom_set_value:
            {
                custom_set_value(value_id_and_data);
                break;
            }
            case id_custom_get_value:
            {
                custom_get_value(value_id_and_data);
                break;
            }
            case id_custom_save:
            {
                // values are saved in custom_set_value()
                break;
            }
            default:
            {
                // Unhandled message.
                *command_id = id_unhandled;
                break;
            }
        }
        return;
    }

    *command_id = id_unhandled;

    // DO NOT call raw_hid_send(data,length) here, let caller do this
}
#endif

void read_host_led_state(void) {
  led_t led_state = host_keyboard_led_state();
  if (led_state.num_lock)    {
    if (led_numlock == false){
    led_numlock = true;}
    } else {
    if (led_numlock == true){
    led_numlock = false;}
    }
  if (led_state.caps_lock)   {
    if (led_capslock == false){
    led_capslock = true;}
    } else {
    if (led_capslock == true){
    led_capslock = false;}
    }
  if (led_state.scroll_lock) {
    if (led_scrolllock == false){
    led_scrolllock = true;}
    } else {
    if (led_scrolllock == true){
    led_scrolllock = false;}
    }
}

layer_state_t layer_state_set_kb(layer_state_t state) {
  state = layer_state_set_user(state);
  layer = get_highest_layer(state);
  oled_request_wakeup();
  return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  oled_request_wakeup();
  switch (keycode) {
    case OLED_TOGG:
      if(!clock_set_mode){
        if (record->event.pressed) {
          oled_mode = (oled_mode + 1) % _NUM_OLED_MODES;
        }
      }
      return false;
    case CLOCK_SET:
      if (record->event.pressed) {
        if(clock_set_mode){
          pre_encoder_mode_change();
          clock_set_mode = false;
          encoder_mode = previous_encoder_mode;
          post_encoder_mode_change();

        }else{
          previous_encoder_mode = encoder_mode;
          pre_encoder_mode_change();
          clock_set_mode = true;
          encoder_mode = ENC_MODE_CLOCK_SET;
          post_encoder_mode_change();
        }
      }
      return false;
    case ENC_PRESS:
      if (record->event.pressed) {
        uint16_t mapped_code = handle_encoder_press();
        uint16_t held_keycode_timer = timer_read();
        if(mapped_code != 0){
          register_code16(mapped_code);
          while (timer_elapsed(held_keycode_timer) < MEDIA_KEY_DELAY){ /* no-op */ }
          unregister_code16(mapped_code);
        }
      } else {
        // Do something else when release
      }
      return false;
    default:
      break;
  }

  return process_record_user(keycode, record);
}


bool encoder_update_kb(uint8_t index, bool clockwise) {
    if (!encoder_update_user(index, clockwise)) return false;
    oled_request_wakeup();
  encoder_value = (encoder_value + (clockwise ? 1 : -1)) % 64;
  if (index == 0) {
    if (layer == 0){
      uint16_t mapped_code = 0;
      if (clockwise) {
        mapped_code = handle_encoder_clockwise();
      } else {
        mapped_code = handle_encoder_ccw();
      }
      uint16_t held_keycode_timer = timer_read();
      if(mapped_code != 0){
        register_code16(mapped_code);
        while (timer_elapsed(held_keycode_timer) < MEDIA_KEY_DELAY){ /* no-op */ }
        unregister_code16(mapped_code);
      }
    } else {
      if(clockwise){
        change_encoder_mode(false);
      } else {
        change_encoder_mode(true);
      }
    }
  }
  return true;
}

void custom_config_reset(void){
  for(int i = 0; i < VIA_EEPROM_CUSTOM_CONFIG_SIZE; ++i) {
    uint8_t dummy = 0;
    write_custom_config(&dummy, i, 1);
  }

  uint8_t encoder_modes = 0x1F;
  write_custom_config(&encoder_modes, EEPROM_ENABLED_ENCODER_MODES_OFFSET, 1);
}

void custom_config_load(void){
#ifdef DYNAMIC_KEYMAP_ENABLE
  read_custom_config(&oled_mode, EEPROM_DEFAULT_OLED_OFFSET, 1);
  read_custom_config(&enabled_encoder_modes, EEPROM_ENABLED_ENCODER_MODES_OFFSET, 1);
#endif
}

// Called from via_init() if VIA_ENABLE
// Called from matrix_init_kb() if not VIA_ENABLE
void satisfaction_core_init(void)
{
  // This checks both an EEPROM reset (from bootmagic lite, keycodes)
  // and also firmware build date (from via_eeprom_is_valid())
  if (eeconfig_is_enabled()) {
    custom_config_load();
  } else	{
#ifdef DYNAMIC_KEYMAP_ENABLE
    // Reset the custom stuff
    custom_config_reset();
#endif
    // DO NOT set EEPROM valid here, let caller do this
  }
}

void matrix_init_kb(void)
{
#ifndef VIA_ENABLE
  satisfaction_core_init();
#endif // VIA_ENABLE

  rtcGetTime(&RTCD1, &last_timespec);
  matrix_init_user();
  oled_request_wakeup();
}

#ifdef VIA_ENABLE
void via_init_kb(void) {
    satisfaction_core_init();
}
#endif // VIA_ENABLE

void housekeeping_task_kb(void) {
  rtcGetTime(&RTCD1, &last_timespec);
  uint16_t minutes_since_midnight = last_timespec.millisecond / 1000 / 60;

  if (minutes_since_midnight != last_minute){
    last_minute = minutes_since_midnight;
    oled_request_repaint();
  }
}
