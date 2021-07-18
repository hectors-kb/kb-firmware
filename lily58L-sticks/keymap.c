 /* Copyright 2017 F_YUUCHI
  * Copyright 2020 Drashna Jaelre <@drashna>
  * Copyright 2020 Ben Roesner (keycapsss.com)
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
  
#include QMK_KEYBOARD_H
#include "analog.h"
#include "joystick.h"
#include <stdio.h>
#include <string.h>
#include <print.h>
#include <transactions.h>


extern uint8_t is_master;

// for centring joysticks
int16_t X_CTR;
int16_t Y_CTR;
int16_t SLAVE_X_CTR;
int16_t SLAVE_Y_CTR;
uint16_t counter = 0;
// will be used for converting joystick movement to keycodes
bool X_FLAG_R = false ; 
bool X_FLAG_L = false ; 
bool Y_FLAG_U = false ;
bool Y_FLAG_D = false ;
bool XS_FLAG_R = false ;
bool XS_FLAG_L = false ;
bool YS_FLAG_U = false ;
bool YS_FLAG_D = false ;
// for initialising slave half joystick
bool SLAVE_SET = false ;


enum layers {
    _QWERTY,
    _FUNC,
    _JS,
    _ADJUST,
};

enum custom_keycodes {
	TEST1 = SAFE_RANGE,
	TEST2,
};



// This governs the behaviour of the left thumbstick switch
void dip_switch_update_user(uint8_t index, bool active)
{
	switch (index) {
		case 0:
			if(active) {
				register_code(KC_SPC);
			} else {
				unregister_code(KC_SPC);
			} break;
	}
}
					

#define FUNC MO(_FUNC)
#define JS TG(_JS)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  |  -   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  =   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI | FUNC | / None  /       \Space \  | FUNC |BackSP| Enter |
 *                   |      |      |      |/(stick)/         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                     KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                     KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_EQL,
  KC_LCTRL, KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                     KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, KC_LBRC,  KC_RBRC,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,  KC_RSFT,
                             KC_LALT, KC_LGUI, FUNC, KC_NO,    KC_SPC,   FUNC,    KC_BSPC, KC_ENT
),
/* FUNC
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  `   |  F1  |  F2  |  F3  |  F4  |  F5  |                    |  F6  |  F7  |  F8  |  F9  |  F10 | F11  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |RGB ON| HUE+ | SAT+ | VAL+ |      |                    | NLCK |  7   |  8   |  9   |  /   | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      | MODE | HUE- | SAT- | VAL- |      |-------.    ,-------|   .  |  4   |  5   |  6   |  *   |   -  |
 * |------+------+------+------+------+------|   [   |    |   ]   |------+------+------+------+------+------|
 * |      |      |      |      |      |  JS  |-------|    |-------|   0  |  1   |  2   |  3   |  +   |      |
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |      | / None  /       \Enter \  |      |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
[_FUNC] = LAYOUT(
  KC_GRV,  KC_F1, 	KC_F2, 	 KC_F3,   KC_F4,   KC_F5,                     KC_F6,   KC_F7, KC_F8, KC_F9, KC_F10,  KC_F11,
  KC_TRNS, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, KC_TRNS,                   KC_NLCK, KC_P7, KC_P8, KC_P9, KC_PSLS, KC_F12,
  KC_TRNS, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, KC_TRNS,                   KC_PDOT, KC_P4, KC_P5, KC_P6, KC_PAST, KC_PMNS,
  _______, _______, _______, _______, _______, JS, 		_______, _______, KC_P0,   KC_P1, KC_P2, KC_P3, KC_PPLS, KC_TRNS,
                             _______, _______, _______, _______, _______,  _______, _______, _______
),
/* JOYSTICK LAYER
 * ,------------------------------------.                   ,--------------------------------------.
 * | BASE |  1  |  2  |  3  |  4  |  5  |                   |  6  |  7  |  8  |  9  |  0  |   -    |
 * |------+-----+-----+-----+-----+-----|                   |-----+-----+-----+-----+-----+--------|
 * |  Tab |  B0 |  B1 | B2  |  B3 | B4  |                   | B5  |  B6 |  B7 |  B8 |  B9 |   =    |
 * |------+-----+-----+-----+-----+-----|                   |-----+-----+-----+-----+-----+--------|
 * | LCTRL| B10 | B11 | B12 | B13 | B14 |-------.    ,------| B15 | B16 | B17 | B18 | B19 |   '    |
 * |------+-----+-----+-----+-----+-----|   [   |    |   ]  |-----+-----+-----+-----+-----+--------|
 * |Lshift|  Z  |  X  |  C  |  V  |  B  |-------|    |------|  N  |  M  |  ,  |  .  |  /  | Rshift |
 * `--------------------------------------/    /     \      \--------------------------------------'
 *                 | LAlt | LGUI |LOWER| /None/       \Enter \ |RAISE |BackSP| RGUI |
 *                 |      |      |     |/    /         \      \|      |      |      |
 *                 `------------------------'           '-----''--------------------'
 */
  [_JS] = LAYOUT(
  TO(_QWERTY),  KC_TRNS, 	 KC_TRNS, 	  KC_TRNS, 	   KC_TRNS, 	KC_TRNS,                 	   KC_TRNS, 	KC_TRNS, 	 KC_TRNS, 	  KC_TRNS, 	   KC_TRNS, 	KC_TRNS,
  KC_TRNS, 		JS_BUTTON0,  JS_BUTTON1,  JS_BUTTON2,  JS_BUTTON3,  JS_BUTTON4,                    JS_BUTTON5, 	JS_BUTTON6,  JS_BUTTON7,  JS_BUTTON8,  JS_BUTTON9, 	KC_TRNS,
  KC_TRNS, 		JS_BUTTON10, JS_BUTTON11, JS_BUTTON12, JS_BUTTON13, JS_BUTTON14,                   JS_BUTTON15, JS_BUTTON16, JS_BUTTON17, JS_BUTTON18, JS_BUTTON19, KC_TRNS,
  KC_TRNS, 		KC_TRNS, 	 KC_TRNS, 	  KC_TRNS, 	   KC_TRNS, 	KC_TRNS, 	 KC_TRNS, KC_TRNS, KC_TRNS, 	KC_TRNS, 	 KC_TRNS, 	  KC_TRNS, 	   KC_TRNS, 	KC_TRNS,
										  _______,     _______, 	_______, 	 _______, _______, _______, 	_______, 	 _______
  )
};

//SSD1306 OLED update loop, make sure to enable OLED_DRIVER_ENABLE=yes in rules.mk
#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_master()) {
        return OLED_ROTATION_270;
    } else {
        return OLED_ROTATION_0;
    }
}

void render_lily58_logo(void) {
    static const char PROGMEM lily58_logo[] = {
    // 'logo', 128x32px
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xc0, 0x40, 0x40, 0xc0, 0x80, 0x80, 0x80, 0x00, 0x00,
    0x80, 0xe0, 0x70, 0x3c, 0x0e, 0x06, 0x0e, 0x3c, 0x70, 0xe0, 0x80, 0x00, 0x00, 0xc0, 0xc0, 0x00,
    0xc0, 0xc0, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x80,
    0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0xc0, 0x80, 0x80, 0x80, 0x81, 0x83, 0x83,
    0x07, 0x07, 0x0c, 0x18, 0x70, 0xe0, 0x80, 0x00, 0x00, 0x01, 0xff, 0xfc, 0x80, 0xb6, 0xb6, 0x80,
    0xb0, 0xb0, 0x00, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0xf1, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x00, 0x00, 0x00, 0x30, 0xf0, 0xf0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xf0, 0xf0,
    0x30, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xe1, 0x71, 0x71, 0xf1, 0xf1, 0xe1, 0xc1, 0x81, 0x00, 0x00,
    0x00, 0x00, 0x0c, 0x3f, 0xff, 0xf3, 0xe1, 0xc1, 0xc1, 0x81, 0x81, 0xc3, 0xff, 0x7f, 0x1c, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x20, 0x70, 0x78, 0xdc, 0xcc, 0x86, 0x06, 0x03, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x03, 0x02, 0x06, 0x84, 0xe1, 0xfb, 0x38, 0x1c, 0x0c, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x03, 0x03, 0x06, 0x86, 0xcc, 0xdc, 0x78, 0x70, 0x20, 0x00, 0xff, 0xff, 0x80, 0x80,
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0xff,
    0xff, 0x80, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1f, 0x7e, 0xf8, 0xe0, 0xf0, 0x7e, 0x1f, 0x03, 0x00,
    0x00, 0x00, 0x00, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 0x80, 0x80, 0xc0, 0xe1, 0xff, 0x7f, 0x3f, 0x00,
    0x00, 0x00, 0x3e, 0xff, 0xff, 0xc1, 0xc0, 0x80, 0x81, 0x81, 0xc3, 0xc3, 0xff, 0xfe, 0x3c, 0x00,
    0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x06, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04, 0x06,
    0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0x01, 0x01, 0x03, 0x02, 0x06, 0x06, 0x04, 0x04, 0x04, 0x04,
    0x06, 0x06, 0x06, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x01, 0x01, 0x01, 0x00, 0x00, 0x60, 0x60, 0x70, 0x38, 0x1f, 0x0f, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00
};
    oled_write_raw_P(lily58_logo, sizeof(lily58_logo));
}

// OLED display layer code
void render_default_layer_state(void) {
    oled_write_P(PSTR("Layer"), false);
    oled_write_P(PSTR(" "), false);
    switch (get_highest_layer(layer_state)) {

        case _FUNC:
            oled_write_ln_P(PSTR("FUNC"), false);
            break;
        case _JS:
            oled_write_P(PSTR(" JOY"), false);
            break;

      default:
		oled_write_P(PSTR("QRTY"), false);

    }
}

// OLED display locks code
void render_keylock_status(led_t led_state) {
    oled_write_ln_P(PSTR("Lock"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("N"), led_state.num_lock);
    oled_write_P(PSTR("C"), led_state.caps_lock);
    oled_write_ln_P(PSTR("S"), led_state.scroll_lock);
}

// OLED displays which modifiers are active
void render_mod_status(uint8_t modifiers) {
    oled_write_ln_P(PSTR("Mods"), false);
    oled_write_P(PSTR(" "), false);
    oled_write_P(PSTR("S"), (modifiers & MOD_MASK_SHIFT));
    oled_write_P(PSTR("C"), (modifiers & MOD_MASK_CTRL));
    oled_write_P(PSTR("A"), (modifiers & MOD_MASK_ALT));
    oled_write_P(PSTR("G"), (modifiers & MOD_MASK_GUI));
}


void render_status_main(void) {
    // Show keyboard layout
    render_default_layer_state();
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    // Show host keyboard led status
    render_keylock_status(host_keyboard_led_state());
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
    // Show modifier status
    render_mod_status(get_mods());
    // Add a empty line
    oled_write_P(PSTR("-----"), false);
}

void oled_task_user(void) {
  //update_log();
  if (is_keyboard_master()) {
    render_status_main();  // Renders the current keyboard state (layer, lock, caps, scroll, etc)
  } else {
    render_lily58_logo(); // Displays logo on slave half
  }
}

#endif // OLED_DRIVER_ENABLE

// Rotary encoder related code
#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { // Encoder on master side
      if (clockwise) {
          tap_code(KC_PGUP);
      } else {
          tap_code(KC_PGDN);
      }
  }
  else if (index == 1) { // Encoder on slave side
      if (clockwise) {
          tap_code(KC_END);
      } else {
          tap_code(KC_HOME);
      }
  }
    return 0;
}
#endif


// Define a structure to carry the ADC values from the slave half
typedef struct user_runtime_config
{
    int16_t X_SLAVE;
    int16_t Y_SLAVE;
} user_runtime_config;

user_runtime_config user_state; //initialises an instance of this struct

//function for receiving slave state
void user_state_sync(void)
{
    if (is_keyboard_master()) {
        transaction_rpc_recv(SLAVE_ADC_VALS, sizeof(user_state), &user_state); //SLAVE_ADC_VALS named in "config.h"
}
}

//Reads ADC pins on slave half
void slave_ADC_transport(uint8_t initiator2target_buffer_size, const void* initiator2target_buffer, uint8_t target2initiator_buffer_size, void* target2initiator_buffer)
{
    if (!is_keyboard_master())
    {
        user_state.X_SLAVE = analogReadPin(F0);
        user_state.Y_SLAVE = analogReadPin(F1);
         
        if (target2initiator_buffer_size == sizeof(user_state)) {
            memcpy(target2initiator_buffer, &user_state, sizeof(user_state));
        }
    }
}

// Run at keyboard initialisation
void keyboard_post_init_user(void) {
    transaction_register_rpc(SLAVE_ADC_VALS, slave_ADC_transport);  // register custom slave transport
    Y_CTR = analogReadPin(F0);		// records the joystick ADC values for centring
    X_CTR = analogReadPin(F1);
}

// Initialise joystick axes. The master half axes are also defined virtually to enable runtime centring
joystick_config_t joystick_axes[JOYSTICK_AXES_COUNT] = {
	[0] = JOYSTICK_AXIS_VIRTUAL,
	[1] = JOYSTICK_AXIS_VIRTUAL,
    [2] = JOYSTICK_AXIS_VIRTUAL,
    [3] = JOYSTICK_AXIS_VIRTUAL
};

void housekeeping_task_user(void)
{
    user_state_sync();  //tells the keyboard to continuously receive the ADC values from the slave half
}

/* Initialise the centring of slave half joystick. 
 * It refused to cooperate when I put it in keyboard_post_init_user,
 * so it sets these values the first time a key is pressed.
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
        if (!SLAVE_SET)
        {
            SLAVE_X_CTR = user_state.X_SLAVE;
            SLAVE_Y_CTR = user_state.Y_SLAVE;
            if ( (SLAVE_X_CTR != 0) & (SLAVE_Y_CTR !=0 ) )
            {
                SLAVE_SET = true;
            }
        }
    switch (keycode) {
        default:
            return true;
    }
};

// Reads joysticks every time the keyboard is polled
void matrix_scan_user(void) {
	// converts analogue values for the virtual joystick axes
    int16_t Y = analogReadPin(F0) - Y_CTR;
    int16_t X = analogReadPin(F1) - X_CTR;
	
	/* I put this in as it was occasionally getting erroneous readings around 0 
	 * expected real readings are around 200-1000
	 */
	if (analogReadPin(F1) > 5) 
	{
		joystick_status.axes[0] = - (X / 4);
	}
	joystick_status.axes[1] = Y / 4; 
	// The quotient of 4 is to get it to fit into the axis range of [-127,127]
	if (SLAVE_SET)
    {
    int16_t YS = user_state.Y_SLAVE - SLAVE_Y_CTR;
    int16_t XS = user_state.X_SLAVE - SLAVE_X_CTR;
		if (user_state.X_SLAVE > 5)
		{
			joystick_status.axes[2] = - (XS / 4);
		}
        joystick_status.axes[3] = YS / 4;
        
    }
	joystick_status.status |= JS_UPDATED;
	
	// When not on the joystick layer, this converts the master-side stick to arrow keys
	if ( IS_LAYER_OFF(_JS)) 
	{
		if ( !(X_FLAG_L) && (X > 200)) {
			register_code(KC_LEFT);
			X_FLAG_L = true;
		} else if ( (X_FLAG_L) && (X <= 200)) {
			unregister_code(KC_LEFT);
			X_FLAG_L = false;
		}
		if ( !(X_FLAG_R) && (X <= -200)) {
			register_code(KC_RIGHT);
			X_FLAG_R = true;
		} else if ( (X_FLAG_R) && (X > -200)) {
			unregister_code(KC_RIGHT);
			X_FLAG_R = false;
		}
		if ( !(Y_FLAG_U) && (Y > 200)) {
			register_code(KC_UP);
			Y_FLAG_U = true;
		} else if ( (Y_FLAG_U) && (Y <= 200)) {
			unregister_code(KC_UP);
			Y_FLAG_U = false;
		}
		if ( !(Y_FLAG_D) && (Y <= -200)) {
			register_code(KC_DOWN);
			Y_FLAG_D = true;
		} else if ( (Y_FLAG_D) && (Y > -200)) {
			unregister_code(KC_DOWN);
			Y_FLAG_D = false;
		}
		/* Insert similar code here to convert the slave-side stick
		 * I have named the corresponding flags XS_FLAG_L, etc.
		 */
    }
        
   
}




    
