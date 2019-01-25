/*	AXIOM Remote
 **
 **	Copyright (C) 2018 Sebastian Pichelhofer
 * 
 **
 **	This program is free software; you can redistribute it and/or modify
 **    	it under the terms of the GNU General Public License 2 as published 
 **	by the Free Software Foundation.
 **
 **	Compile with -O6 for best experience
 */

#include "globals.h"

GFXfont FreeSans9pt7b;
GFXfont FreeSans12pt7b;
GFXfont FreeSans18pt7b;
GFXfont FreeSans24pt7b;

enum menu_id_t current_menu = MENU_MAIN;
enum page_id_t current_page = PAGE_HOME;

page_t main_page[3];

// Buttons and knobs
bool btn_E1_pressed = false;
bool btn_E2_pressed = false;
bool btn_P7_pressed = false;
bool btn_P8_pressed = false;
bool btn_S1_pos;
bool btn_TS1_pos;
bool btn_TS2_pos;
uint8_t E1_pos;
uint8_t E2_pos;

uint16_t _cursor_x = 0;
uint16_t _cursor_y = 0;


