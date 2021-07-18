/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#pragma once


#ifdef KEYBOARD_crkbd_rev1_legacy
#    undef USE_I2C
#    define USE_SERIAL
#endif

/* Select hand configuration */
#define MASTER_LEFT
//#define MASTER_RIGHT
//#define EE_HANDS  
//Note: EE_HANDS not supported yet

#define USE_SERIAL_PD2  //Note: I2C communication not supported yet (clash with the trackball I2C transport?)

//If you are having issues with the Frog layers/combos, it may be worth experimenting with these options
#define TAPPING_TERM 275
#define PERMISSIVE_HOLD
#define COMBO_COUNT 16
#define COMBO_TERM 60
//#define FORCE_NKRO
//#define ONESHOT_TIMEOUT 8000
//---------------------------

#define MOUSEKEY_DELAY 0
#define MOUSEKEY_INTERVAL 28

#define AUTO_SHIFT_TIMEOUT 175
#define NO_AUTO_SHIFT_NUMERIC
#define NO_AUTO_SHIFT_ALPHA


#define OLED_FONT_H "keyboards/crkbd/lib/glcdfont.c"

#ifdef RGB_MATRIX_ENABLE
#	define RGB_MATRIX_SPLIT { 27, 27 }
#endif