/* Copyright 2016 Jack Humbert
 * Copyright 2019 Drashna Jael're (@drashna, aka Christopher Courtney)
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

/* Author: Wojciech Siewierski < wojciech dot siewierski at onet dot pl > */
#include "process_epic_macro.h"


// default feedback method
void epic_macro_led_blink(void) {
#ifdef BACKLIGHT_ENABLE
    backlight_toggle();
    wait_ms(100);
    backlight_toggle();
#endif
}

/* User hooks for Epic Macros */

__attribute__((weak)) void epic_macro_record_start_user(uint8_t macroNr) {
    epic_macro_led_blink();
}

__attribute__((weak)) void epic_macro_play_user(int8_t macro_nr) {
    epic_macro_led_blink();
}

__attribute__((weak)) void epic_macro_record_key_user(int8_t macro_nr, keyrecord_t *record) {
    epic_macro_led_blink();
}

__attribute__((weak)) void epic_macro_record_end_user(int8_t direction) {
    epic_macro_led_blink();
}

__attribute__((weak)) bool epic_macro_valid_key_user(uint16_t keycode, keyrecord_t *record) {
    return true;
}

typedef struct {
  uint8_t len;
  keyrecord_t *keys;
  uint8_t *delays;
} macro_t;

static macro_t *macros[EPIC_MACRO_COUNT];

static keyrecord_t static_keys[EPIC_MACRO_SIZE];
static uint8_t static_delays[EPIC_MACRO_SIZE];
static macro_t static_macro = {
  .len = 0,
  .keys = static_keys,
  .delays = static_delays
};

static macro_t *current_macro = 0;
static int8_t current_macro_nr = -1;

bool epic_macro_active(uint8_t macro_nr) {
  return macros[macro_nr] != 0;
}

void epic_macro_record_start(uint8_t macro_nr) {
    if (macro_nr >= EPIC_MACRO_COUNT) return;
    dprintf("epic macro recording: started for macro nr %u\n", macro_nr);

    epic_macro_record_start_user(macro_nr);

    clear_keyboard();
    layer_clear();
    current_macro = &static_macro;
    current_macro->len = 0;
    current_macro_nr = macro_nr;
}

void epic_macro_play(uint8_t macro_nr) {
    if (macro_nr >= EPIC_MACRO_COUNT || macros[macro_nr] == 0) return;
    dprintf("epic macro: slot %d playback\n", macro_nr);

    layer_state_t saved_layer_state = layer_state;

    clear_keyboard();
    layer_clear();

    macro_t *m = macros[macro_nr];
    for (uint8_t i = 0; i < m->len; i++) {
      process_record(&m->keys[i]);
      wait_ms(&m->delays[i]);
    }

    clear_keyboard();

    layer_state_set(saved_layer_state);

    epic_macro_play_user(macro_nr);
}

void epic_macro_record_key(keyrecord_t *record) {
    if (current_macro == 0) return;
    /* If we've just started recording, ignore all the key releases. */
    if (!record->event.pressed && current_macro->len == 0) {
        dprintln("epic macro: ignoring a leading key-up event");
        return;
    }

    if (current_macro->len < EPIC_MACRO_SIZE) {
        current_macro->keys[current_macro->len] = *record;
        current_macro->len++;
    } else {
        epic_macro_record_key_user(current_macro_nr, record);
    }

    dprintf("epic macro: slot %d length: %d/%d\n", current_macro_nr, current_macro->len, EPIC_MACRO_SIZE);
}

void epic_macro_record_end(keyrecord_t *macro_buffer, keyrecord_t *macro_pointer, int8_t direction, keyrecord_t **macro_end) {
    /* Do not save the keys being held when stopping the recording,
     * i.e. the keys used to access the layer DM_RSTP is on.
     */
    uint8_t len = current_macro->len;
    while (len > 0 && current_macro->keys[len].event.pressed) {
        dprintln("epic macro: trimming a trailing key-down event");
        len--;
    }

    dprintf("epic macro: slot %d saved, length: %d\n", current_macro_nr, current_macro->len, EPIC_MACRO_SIZE);

    macro_t *m = macros[current_macro_nr] = malloc(sizeof(macro_t));
    m = malloc(sizeof(macro_t));
    m->len = len;
    m->keys = malloc(sizeof(keyrecord_t)*len);
    m->delays = malloc(sizeof(keyrecord_t)*len);
    memcpy(m->keys, current_macro->keys, sizeof(keyrecord_t)*len);
    memcpy(m->delays, current_macro->delays, sizeof(uint8_t)*len);
    macros[current_macro_nr] = m;
    current_macro = 0;
    current_macro_nr = -1;

    epic_macro_record_end_user(current_macro_nr);
}



/* Handle the key events related to the epic macros. Should be
 * called from process_record_user() like this:
 *
 *   bool process_record_user(uint16_t keycode, keyrecord_t *record) {
 *       if (!process_record_epic_macro(keycode, record)) {
 *           return false;
 *       }
 *       <...THE REST OF THE FUNCTION...>
 *   }
 */
bool process_epic_macro(uint16_t keycode, keyrecord_t *record) {
    if (current_macro == 0) {
        /* No macro recording in progress. */
        if (!record->event.pressed) {
            switch (keycode) {
                case QK_EPIC_MACRO_RECORD_START_0: epic_macro_record_start(0); return false;
                case QK_EPIC_MACRO_RECORD_START_1: epic_macro_record_start(1); return false;
                case QK_EPIC_MACRO_RECORD_START_2: epic_macro_record_start(2); return false;
                case QK_EPIC_MACRO_RECORD_START_3: epic_macro_record_start(3); return false;
                case QK_EPIC_MACRO_RECORD_START_4: epic_macro_record_start(4); return false;
                case QK_EPIC_MACRO_RECORD_START_5: epic_macro_record_start(5); return false;
                case QK_EPIC_MACRO_RECORD_START_6: epic_macro_record_start(6); return false;
                case QK_EPIC_MACRO_RECORD_START_7: epic_macro_record_start(7); return false;
                case QK_EPIC_MACRO_RECORD_START_8: epic_macro_record_start(8); return false;
                case QK_EPIC_MACRO_RECORD_START_9: epic_macro_record_start(9); return false;
                case QK_EPIC_MACRO_RECORD_START_0: epic_macro_record_start(0); return false;
                case QK_EPIC_MACRO_PLAY_0: epic_macro_play(0); return false;
                case QK_EPIC_MACRO_PLAY_1: epic_macro_play(1); return false;
                case QK_EPIC_MACRO_PLAY_2: epic_macro_play(2); return false;
                case QK_EPIC_MACRO_PLAY_3: epic_macro_play(3); return false;
                case QK_EPIC_MACRO_PLAY_4: epic_macro_play(4); return false;
                case QK_EPIC_MACRO_PLAY_5: epic_macro_play(5); return false;
                case QK_EPIC_MACRO_PLAY_6: epic_macro_play(6); return false;
                case QK_EPIC_MACRO_PLAY_7: epic_macro_play(7); return false;
                case QK_EPIC_MACRO_PLAY_8: epic_macro_play(8); return false;
                case QK_EPIC_MACRO_PLAY_9: epic_macro_play(9); return false;
            }
        }
    } else {
        /* A macro is being recorded right now. */
        switch (keycode) {
            case QK_EPIC_MACRO_RECORD_START_0:
            case QK_EPIC_MACRO_RECORD_START_1:
            case QK_EPIC_MACRO_RECORD_START_2:
            case QK_EPIC_MACRO_RECORD_START_3:
            case QK_EPIC_MACRO_RECORD_START_4:
            case QK_EPIC_MACRO_RECORD_START_5:
            case QK_EPIC_MACRO_RECORD_START_6:
            case QK_EPIC_MACRO_RECORD_START_7:
            case QK_EPIC_MACRO_RECORD_START_8:
            case QK_EPIC_MACRO_RECORD_START_9:
            case QK_EPIC_MACRO_RECORD_START_0:
            case QK_EPIC_MACRO_RECORD_STOP:
                /* Stop the macro recording. */
                if (record->event.pressed ^ (keycode != QK_EPIC_MACRO_RECORD_STOP)) { /* Ignore the initial release
                                                                                          * just after the recording
                                                                                          * starts for DM_RSTP. */
                    epic_macro_record_end(macro_nr);
                }
                return false;
#ifdef EPIC_MACRO_NO_NESTING
            case QK_EPIC_MACRO_PLAY_1:
            case QK_EPIC_MACRO_PLAY_2:
                dprintln("epic macro: ignoring macro play key while recording");
                return false;
#endif
            default:
                if (epic_macro_valid_key_user(keycode, record)) {
                    /* Store the key in the macro buffer and process it normally. */
                    epic_macro_record_key(macro_nr, record);
                }
                return true;
                break;
        }
    }

    return true;
}
