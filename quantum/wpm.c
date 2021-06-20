/*
 * Copyright 2020 Richard Sutherland (rich@brickbots.com)
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

#include "wpm.h"

bool wpm_keycode(uint16_t keycode) { return wpm_keycode_kb(keycode); }

__attribute__((weak)) bool wpm_keycode_kb(uint16_t keycode) { return wpm_keycode_user(keycode); }

__attribute__((weak)) bool wpm_keycode_user(uint16_t keycode) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) || (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
        keycode = keycode & 0xFF;
    } else if (keycode > 0xFF) {
        keycode = 0;
    }
    if ((keycode >= KC_A && keycode <= KC_0) || (keycode >= KC_TAB && keycode <= KC_SLASH)) {
        return true;
    }

    return false;
}

// Full range is 100 * 100 ms = 10 s
// The computed WPM needs to be multiplied by 6 to get the actual WPM
#define WPM_WINDOW_NUM 100
#define WPM_WINDOW_WPMMAX 5
#define WPM_WINDOW_MS 100

#define WPM_PROCESS_DECAY 0
#define WPM_PROCESS_KEYPRESS 1

typedef struct {
    uint8_t  windows[WPM_WINDOW_NUM];
    uint8_t  period;
    uint8_t  word;
    uint8_t  wpm;
    uint8_t  wpm_displayed;
    uint16_t timer;
} wpm_state_t;

static wpm_state_t s_wpm_state = {.period = -1, .word = 0, .wpm = 0, .wpm_displayed = 0};

// This should only be called on the slave side (when receiving I2C data)
void set_current_wpm(uint8_t new_wpm) { s_wpm_state.wpm_displayed = new_wpm; }

uint8_t get_current_wpm(void) { return s_wpm_state.wpm_displayed; }

void process_wpm(uint8_t mode) {
    uint16_t elapsed = timer_elapsed(s_wpm_state.timer);

    s_wpm_state.word += mode;
    if (elapsed >= WPM_WINDOW_MS) {
        // TODO: Can we move this to some initialization step?
        if (s_wpm_state.period == -1) {
            for (int8_t i = 0; i < WPM_WINDOW_NUM; i++) {
                s_wpm_state.windows[i] = 0;
            }
        }

        s_wpm_state.timer = timer_read();
        s_wpm_state.period++;
        if (s_wpm_state.period == WPM_WINDOW_NUM) {
            s_wpm_state.period = 0;
        }

        if (mode == WPM_PROCESS_DECAY) {
            // Have the wpm display value move towards the actual (lower) wpm value in a timely manner
            if ((s_wpm_state.period & 0x7) == 0) {                              // Delaying by 8 * WPM_WINDOW_MS = 800 ms
                uint8_t wpm = (s_wpm_state.wpm << 2) + (s_wpm_state.wpm << 1);  // * 6
                if (wpm < s_wpm_state.wpm_displayed) {
                    s_wpm_state.wpm_displayed = s_wpm_state.wpm_displayed - (((s_wpm_state.wpm_displayed - wpm) >> 2) | 1);
                }
            }
        }

        s_wpm_state.wpm -= s_wpm_state.windows[s_wpm_state.period];
        s_wpm_state.windows[s_wpm_state.period] = 0;
    }
    if (s_wpm_state.word >= 5) {
        s_wpm_state.windows[s_wpm_state.period] += 1;
        s_wpm_state.wpm += 1;
        s_wpm_state.word -= 5;
    }
}

void update_wpm(uint16_t keycode) {
    if (wpm_keycode(keycode)) {
        process_wpm(WPM_PROCESS_KEYPRESS);
        {
            // Have the wpm display value move towards the actual (higher) wpm value in a rapid manner
            uint8_t wpm = (s_wpm_state.wpm << 2) + (s_wpm_state.wpm << 1);
            if (wpm > s_wpm_state.wpm_displayed) {
                s_wpm_state.wpm_displayed = wpm;
            }
        }
    }
}

void decay_wpm(void) { process_wpm(WPM_PROCESS_DECAY); }