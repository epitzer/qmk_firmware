/* Copyright 2023 @ Keychron (https://www.keychron.com)
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

/* added features (epi)
 * extra layer
 * top layer (boss key toggle)
 * home/end <-> pgup/pgdn
 * fn+del -> ins
 * fn+space -> leader
 */

// TODO: blinking LED while recording macro https://docs.qmk.fm/#/feature_dynamic_macros?id=user-hooks
// TODO: indicate startup complete
// TODO: accelerate init

#include QMK_KEYBOARD_H
#include "print.h"

#ifdef CONSOLE_ENABLE
#define DEBUG(X, ...) if (debug_enable) { uprintf(X, ##__VA_ARGS__); }
#else
#define DEBUG(X, ...)
#endif

// clang-format off
enum layers{
  L_MAC_BASE,
  L_MAC_FN,
  L_BASE,
  L_MOUSE,
  L_NUMP,
  L_FN,
  L_EXTRA,
  L_TOP
};


enum custom_keycodes {
    EP_CMF2 = SAFE_RANGE, /// Control+Meta+F2
    EP_SHLY, /// show layers
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
[L_MAC_BASE] = LAYOUT_iso_85(
  KC_ESC,   KC_BRID,  KC_BRIU,  KC_MCTL,  KC_LNPD,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_SNAP,  KC_DEL,   RGB_MOD,
  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_ENT,             KC_PGDN,
  KC_CAPS,  KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_BSLS,            KC_HOME,
  KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_END,
  KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD,MO(L_MAC_FN),KC_RCTL,KC_LEFT,  KC_DOWN,  KC_RGHT),

[L_MAC_FN] = LAYOUT_iso_85(
  _______,  KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   _______,  _______,  RGB_TOG,
  _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  RGB_TOG,  RGB_MOD,  RGB_VAI,  RGB_HUI,  RGB_SAI,  RGB_SPI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  RGB_RMOD, RGB_VAD,  RGB_HUD,  RGB_SAD,  RGB_SPD,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  NK_TOGG,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),


#define EP_MCAPS LT(L_MOUSE, KC_CAPS)

[L_BASE] = LAYOUT_iso_85(
  KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_PSCR,  KC_DEL,   DM_PLY2,
  KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_HOME,
  KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,  KC_ENT,             KC_END,
  EP_MCAPS, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,            KC_BSLS,            KC_PGUP,
  KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,    KC_PGDN,
  KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT, MO(L_FN),  KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT),

[L_MOUSE] = LAYOUT_iso_85(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  KC_ACL0,  KC_ACL1,  KC_ACL2,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  KC_BTN1,  KC_BTN3,  KC_BTN2,  _______,  _______,  _______,  KC_BTN1,  KC_BTN3,  KC_BTN2,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  KC_MS_L,  KC_MS_D,  KC_MS_U,  KC_MS_R,  _______,  _______,            _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_WH_D,  KC_WH_U,  _______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[L_NUMP] = LAYOUT_iso_85(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  KC_KP_1,  KC_KP_2,  KC_KP_3,  KC_KP_4,  KC_KP_5,  KC_KP_6,  KC_KP_7,  KC_KP_8,  KC_KP_9,  KC_KP_0,  KC_PMNS,  KC_PPLS,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_PENT,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_NUM,   _______,  _______,  KC_PAST,  KC_PSLS,            _______,  _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[L_FN] = LAYOUT_iso_85(
  _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  RGB_VAD,  RGB_VAI,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_SCRL,  KC_INS,   DM_REC2,
  _______,  BT_HST1,  BT_HST2,  BT_HST3,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  RGB_TOG,  _______,  _______,  RGB_HUI,  RGB_SAI,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  RGB_HUD,  RGB_SAD,  _______,  _______,  _______,  _______,  EP_SHLY,  _______,  _______,            _______,            _______,
  _______,  _______,  _______, TG(L_TOP), _______,  _______, BAT_LVL,TG(L_NUMP),TG(L_MOUSE),_______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,                                QK_LEAD,                            MO(L_EXTRA),  _______,  _______,  _______,  _______,  _______),

[L_EXTRA] = LAYOUT_iso_85(
  KC_PWR ,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_SYRQ,  _______,  _______,
  _______,  DM_PLY1,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  DM_REC1,  _______,  _______,  QK_RBT,   _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  DB_TOGG,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  QK_BOOT,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),

[L_TOP] = LAYOUT_iso_85(
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,            _______,
  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,  _______,
  _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______),
};


// clang-format on
bool is_scroll_lock = 0;
bool show_layers = 0;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  DEBUG("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n",
         keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
  switch (keycode) {
      // case EP_CMF2: if (record->event.pressed) SEND_STRING(SS_LCTL(SS_LALT(SS_TAP(X_F2)))); break;
  case KC_SCRL: if (record->event.pressed) is_scroll_lock = !is_scroll_lock; break;
  case EP_SHLY: if (record->event.pressed) show_layers = !show_layers; break;
  }
  return true;
};

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
    for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
      uint8_t index = g_led_config.matrix_co[row][col];

      if (index >= led_min && index < led_max && index != NO_LED) {
        uint16_t layer = layer_switch_get_layer(MAKE_KEYPOS(row, col));
        uint16_t keycode = keymap_key_to_keycode(L_BASE, MAKE_KEYPOS(row, col));
        if (show_layers) {
            if ((keycode == KC_0 && layer_state_is(0)) ||
                (keycode == KC_1 && layer_state_is(1)) ||
                (keycode == KC_2 && layer_state_is(2)) ||
                (keycode == KC_3 && layer_state_is(3)) ||
                (keycode == KC_4 && layer_state_is(4)) ||
                (keycode == KC_5 && layer_state_is(5)) ||
                (keycode == KC_6 && layer_state_is(6)) ||
                (keycode == KC_7 && layer_state_is(7)) ||
                (keycode == KC_8 && layer_state_is(8)))
              rgb_matrix_set_color(index, RGB_GREEN);
        }
        if ((keycode == KC_LSFT || keycode == KC_RSFT) && is_caps_word_on()) {
          rgb_matrix_set_color(index, RGB_BLUE);
        } else if (keycode == KC_N && host_keyboard_led_state().num_lock) {
          rgb_matrix_set_color(index, RGB_BLUE);
        } else if (keycode == KC_PSCR && (is_scroll_lock || host_keyboard_led_state().scroll_lock)) {
          rgb_matrix_set_color(index, RGB_BLUE);
        } else if (keycode == KC_LSFT && host_keyboard_led_state().caps_lock) {
          rgb_matrix_set_color(index, RGB_BLUE);
        } else switch (layer) {
          case L_MAC_BASE | L_MAC_FN: rgb_matrix_set_color(index, RGB_WHITE); break; // i.e. wrong layer ;-)
          case L_FN: rgb_matrix_set_color(index, 0xFF, 0x00, 0x40); break;
          case L_EXTRA: rgb_matrix_set_color(index, 0x40, 0x00, 0xFF); break;
          case L_MOUSE: rgb_matrix_set_color(index, 0x00, 0xFF, 0x00); break;
          case L_NUMP: rgb_matrix_set_color(index, 0xFF, 0xFF, 0xFF); break;
          case L_TOP: rgb_matrix_set_color(index, 0xFF, 0xFF, 0xFF); break;
              // case L_BASE: // do nothing
              // default:
        }
      }
    }
  }
  return false;
}


#ifdef LEADER_ENABLE

LEADER_EXTERNS();

void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();
    SEQ_TWO_KEYS(KC_M, KC_1) {}

    // SEQ_ONE_KEY(KC_L) {}
    SEQ_TWO_KEYS(KC_L, KC_S) {
      // show_active_layers();
    }
    SEQ_TWO_KEYS(KC_L, KC_T) {
      // toggle_active_layers(); // visually
    }
  }
}

#endif
