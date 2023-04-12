#pragma once

#include "quantum.h"

/* May be overridden with a custom value. Be aware that the effective
 * macro length is half of this value: each keypress is recorded twice
 * because of the down-event and up-event. This is not a bug, it's the
 * intended behavior.
 *
 * Usually it should be fine to set the macro size to at least 256 but
 * there have been reports of it being too much in some users' cases,
 * so 128 is considered a safe default.
 */
#ifndef EPIC_MACRO_SIZE
#    define EPIC_MACRO_SIZE 128
#endif
#ifndef EPIC_MACRO_COUNT
#    define EPIC_MACRO_COUNT 10
#endif

void epic_macro_led_blink(void);
bool process_epic_macro(uint16_t keycode, keyrecord_t *record);
void epic_macro_record_start_user(uint8_t macro_nr);
void epic_macro_play_user(int8_t macro_nr);
void epic_macro_record_key_user(int8_t direction, keyrecord_t *record);
void epic_macro_record_end_user(int8_t direction);
