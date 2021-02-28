#pragma once

#ifdef AUDIO_ENABLE
    #define STARTUP_SONG SONG(PREONIC_SOUND)
    #define GOODBYE_SONG SONG(GOODBYE_SOUND)

    
    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND) }
#endif

#define MUSIC_MASK (keycode != KC_NO)

// ENCODER options

#define ENCODER_DIRECTION_FLIP
#define ENCODER_RESOLUTION 4
#define TAP_CODE_DELAY 10
#define AUDIO_CLICKY

// COMBO options

#define COMBO_COUNT 1