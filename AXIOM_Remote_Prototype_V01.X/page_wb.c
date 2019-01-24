
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

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "definitions.h"
#include "globals.h"
#include "utility.h"


//#include "main.c"

#ifndef PAGE_WB_C
#define PAGE_WB_C

// Color Definitions
uint16_t _page_item_label_color;
uint16_t _page_item_value_color;
uint16_t _page_item_label_background_color;
uint16_t _page_item_value_background_color;
uint16_t _page_item_highlight_color;
uint16_t _page_background_color;
uint16_t _page_options_background_color;
uint16_t _button_primary_background_color;
uint16_t _button_background_color;
uint16_t _button_hightlight_background_color;
uint16_t _page_item_width;
uint8_t _padding_side;
uint8_t _padding_elements;
uint8_t _page_item_label_height;
uint8_t _page_item_value_height;

//menu_t _wb_menu;
uint8_t _wb_menu_highlight_index;



/**************************************************************************/
/*!
    @brief    draw a standard page item that contains a value and label 
    @param    screen_index - position of the item on screen, there is space for 3 top and 3 bottom, total 6 items
    @param    page_item - the struct containing details about the page item
    @param    highlighted - is the item currently highlighted
 */

/**************************************************************************/
void draw_wb_page_item(uint8_t screen_index) {

    // don't draw empty or disabled items
    if ((_main_page[PAGE_WB].page_item[screen_index].label == NULL) || (_main_page[PAGE_WB].page_item[screen_index].disabled)) {
        return;
    }

    // the screen only provides space for 6 items
    screen_index = limit_range(screen_index, 0, 5);

    uint16_t page_item_height = _page_item_label_height + _page_item_value_height;

    // derive x,y coordinates from screen_index
    uint16_t x, y;
    if (screen_index == 0) {
        x = _padding_side;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 1) {
        x = _padding_side + _padding_elements + _page_item_width;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 2) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_item_width;
        y = FRAMEBUFFER_TOP - page_item_height;
    }
    if (screen_index == 3) {
        x = _padding_side;
        y = 0;
    }
    if (screen_index == 4) {
        x = _padding_side + _padding_elements + _page_item_width;
        y = 0;
    }
    if (screen_index == 5) {
        x = _padding_side + 2 * _padding_elements + 2 * _page_item_width;
        y = 0;
    }

    // handle item highlighting
    uint16_t page_item_label_color;
    uint16_t page_item_label_background_color;
    uint16_t page_item_value_background_color;
    uint16_t page_item_value_color;
    if (_main_page[PAGE_WB].page_item[screen_index].highlighted) {
        page_item_label_color = _page_item_label_background_color;
        page_item_label_background_color = _button_hightlight_background_color;
        page_item_value_color = _page_item_label_color;
        page_item_value_background_color = _page_item_label_background_color;
    } else {
        page_item_label_color = _main_page[PAGE_WB].page_item[screen_index].text_color;
        page_item_label_background_color = _main_page[PAGE_WB].page_item[screen_index].background_color; //_page_item_label_background_color;
        page_item_value_background_color = _page_item_value_background_color;
        page_item_value_color = _page_item_value_color;
    }

    if (screen_index < 3) {
        // 3 top items

        if (_main_page[PAGE_WB].page_item[screen_index].item_type == BUTTON) {
            fill_round_rect(x, y + 2 + button_height, _page_item_width, button_height, 3, page_item_label_background_color);
            draw_string(x, y + 9 + button_height, _main_page[PAGE_WB].page_item[screen_index].label,
                    page_item_label_color, page_item_label_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_item_width);
        }
        if (_main_page[PAGE_WB].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label
            fill_round_rect(x, y + _page_item_value_height, _page_item_width, _page_item_label_height, 3,
                    page_item_label_background_color);
            fill_rect(x, y + _page_item_value_height, _page_item_width, 3, 3, page_item_label_background_color);
            draw_string(x, y + _page_item_value_height + 7, _main_page[PAGE_WB].page_item[screen_index].label,
                    page_item_label_color, page_item_value_color,
                    _FreeSans9pt7b, TEXT_ALIGN_CENTER, _page_item_width);

            // draw value
            fill_round_rect(x, y, _page_item_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_value_height - 3, _page_item_width, 3, page_item_value_background_color);
            draw_string(x, y + 10, _main_page[PAGE_WB].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_item_width);
        }
    } else {
        // 3 bottom items

        if (_main_page[PAGE_WB].page_item[screen_index].item_type == BUTTON) {
            fill_round_rect(x, y + 2, _page_item_width, button_height, 3, page_item_label_background_color);
            draw_string(x, y + 9, _main_page[PAGE_WB].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_item_width);
        }
        if (_main_page[PAGE_WB].page_item[screen_index].item_type == VALUE_AND_LABEL) {
            // draw label

            fill_round_rect(x, y + 1, _page_item_width, _page_item_label_height, 3, page_item_label_background_color);
            fill_rect(x, y + _page_item_label_height - 3, _page_item_width, 3, 3, page_item_label_background_color);
            draw_string(x, y + 7, _main_page[PAGE_WB].page_item[screen_index].label, page_item_label_color, page_item_label_color,
                    _FreeSans9pt7b, TEXT_ALIGN_CENTER, _page_item_width);

            // draw value
            fill_round_rect(x, y + _page_item_label_height, _page_item_width, _page_item_value_height, 3, page_item_value_background_color);
            fill_rect(x, y + _page_item_label_height, _page_item_width, 3, page_item_value_background_color);
            draw_string(x, y + _page_item_label_height + 6, _main_page[PAGE_WB].page_item[screen_index].value, page_item_value_color, page_item_value_color,
                    _FreeSans12pt7b, TEXT_ALIGN_CENTER, _page_item_width);
        }
    }
}

void draw_wb_page_side_items() {
    //left side

    //Home Icon
    if (_main_page[PAGE_WB].page_item[6].highlighted) {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon_highlight.pixel_data), home_icon_highlight.width, home_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 158, (uint16_t*) (home_icon.pixel_data), home_icon.width, home_icon.height);
    }

    // Back Icon
    if (_main_page[PAGE_WB].page_item[7].highlighted) {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon_highlight.pixel_data), back_icon_highlight.width, back_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 108, (uint16_t*) (back_icon.pixel_data), back_icon.width, back_icon.height);
    }

    // Edit/Trash Icon
    if (_main_page[PAGE_WB].page_item[8].highlighted) {
        //drawRGBBitmap(0, 58, (uint16_t*) (back_icon_highlight.pixel_data), back_icon_highlight.width, back_icon_highlight.height);
    } else {
        drawRGBBitmap(0, 58, (uint16_t*) (trash_icon.pixel_data), trash_icon.width, trash_icon.height);
    }

    //right side

    // Up Icon
    if (_main_page[PAGE_WB].page_item[9].highlighted) {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - up_icon_highlight.width, 158, (uint16_t*) (up_icon_highlight.pixel_data), up_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - up_icon.width, 158, (uint16_t*) (up_icon.pixel_data), up_icon.width, up_icon.height);
    }

    // Help Icon
    if (_main_page[PAGE_WB].page_item[10].highlighted) {
        //drawRGBBitmap(_width - 1 - down_icon_highlight.width, 108, (uint16_t*) (down_icon_highlight.pixel_data), down_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - help_icon.width, 108, (uint16_t*) (help_icon.pixel_data), help_icon.width, help_icon.height);
    }

    // Down Icon
    if (_main_page[PAGE_WB].page_item[11].highlighted) {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - down_icon_highlight.width, 58, (uint16_t*) (down_icon_highlight.pixel_data), down_icon_highlight.width, up_icon_highlight.height);
    } else {
        drawRGBBitmap(FRAMEBUFFER_WIDTH - 1 - down_icon.width, 58, (uint16_t*) (down_icon.pixel_data), down_icon.width, down_icon.height);
    }
}

void draw_wb_option_item(uint16_t x, uint16_t y, uint8_t option_item_index, bool selected) {
    // don't draw empty items
    if (_white_balance.white_balance_options[option_item_index].label == NULL) {
        return;
    }

    uint16_t yoffset_label_from_base = 7;

    // is the current line selected (cursor)?
    if (selected) {
        fill_rect(x, y, FRAMEBUFFER_WIDTH - x - 28, 29, _page_item_highlight_color);

        draw_string(x + 5, y + yoffset_label_from_base, _white_balance.white_balance_options[option_item_index].label,
                _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);

        char value_string[8];
        sprintf(value_string, "%dK", _white_balance.white_balance_options[option_item_index].Kelvin);
        draw_string(x + 5 + 120, y + yoffset_label_from_base, value_string,
                _page_item_label_color, _page_item_label_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
        //draw_string(x + 210, y + yoffset_label_from_base, value, _menu_selected_text_color, _menu_selected_text_color, _FreeSans9pt7b, ALIGN_RIGHT, 80);
        return;
    }

    //draw the option item normally
    //fill_rect(x, y, _width, 29, _page_item_value_background_color);
    draw_string(x + 5, y + yoffset_label_from_base, _white_balance.white_balance_options[option_item_index].label,
            _page_item_value_color, _page_item_value_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
    char value_string[32];
    sprintf(value_string, "%dK", _white_balance.white_balance_options[option_item_index].Kelvin);
    draw_string(x + 5 + 120, y + yoffset_label_from_base, value_string,
            _page_item_value_color, _page_item_value_color, _FreeSans9pt7b, TEXT_ALIGN_LEFT, 0);
    //draw_string(x + 210, y + yoffset_label_from_base, value, menu_text_color, menu_text_color, _FreeSans9pt7b, ALIGN_RIGHT, 80);
}
/**************************************************************************/
/*!
    @brief    draw the page
 */

/**************************************************************************/
void draw_wb_page() {
    //clear the screen
    fill_rect(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT, _page_background_color);


    //draw page items
    uint8_t i;
    for (i = 0; i < _main_page[PAGE_WB].page_items_count; i++) {
        draw_wb_page_item(i);
    }


    // draw options area
    uint16_t page_item_height = _page_item_label_height + _page_item_value_height;
    uint16_t available_height = FRAMEBUFFER_HEIGHT - 2 * button_height - 8;

    fill_rect(0, button_height + 4, FRAMEBUFFER_WIDTH, available_height, _page_options_background_color);


    // Draw header
    draw_string(5, FRAMEBUFFER_HEIGHT - 30, _main_page[PAGE_WB].label, _page_item_label_color, _page_item_label_color,
            _FreeSans18pt7b, TEXT_ALIGN_LEFT, 0);

    // separation line
    draw_line(0, FRAMEBUFFER_HEIGHT - 34, FRAMEBUFFER_RIGHT, FRAMEBUFFER_TOP - 34, _page_item_highlight_color);



    //draw rotary knob indicator/illustration
    drawRGBBitmap(37, 76, (uint16_t*) (wheel_illustration.pixel_data), wheel_illustration.width, wheel_illustration.height);


    //draw option items
    if (_wb_menu_highlight_index > 1)
        draw_wb_option_item(110, button_height + 2 + available_height - 38, _wb_menu_highlight_index - 2, false);

    if (_wb_menu_highlight_index > 0)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (1 * 30), _wb_menu_highlight_index - 1, false);

    draw_wb_option_item(110, button_height + 2 + available_height - 38 - (2 * 30), _wb_menu_highlight_index, true);

    if (_wb_menu_highlight_index < _white_balance.white_balance_options_count - 1)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (3 * 30), _wb_menu_highlight_index + 1, false);

    if (_wb_menu_highlight_index < _white_balance.white_balance_options_count - 2)
        draw_wb_option_item(110, button_height + 2 + available_height - 38 - (4 * 30), _wb_menu_highlight_index + 2, false);


    // draw side icons
    draw_wb_page_side_items();
}

/*
char * wb_item_get_current_value(uint8_t menu_item_index) {
    return "Test"; //_wb_menu.menu_item[menu_item_index];
}*/

void init_wb_page() {

    //colors
    _page_item_label_color = color565(255, 255, 255);
    _page_item_value_color = color565(0, 0, 0);
    _page_item_label_background_color = color565(0, 0, 0);
    _page_item_value_background_color = color565(255, 255, 255);
    _page_item_highlight_color = color565(255, 128, 0);

    _page_background_color = color565(97, 92, 91);
    _button_primary_background_color = color565(255, 128, 0);
    _button_hightlight_background_color = color565(0, 128, 255);
    _button_background_color = color565(220, 220, 220);
    _page_options_background_color = color565(255, 255, 255);


    // dimensions
    button_height = 30;
    _padding_side = 8;
    _padding_elements = 11;
    _page_item_width = 94;
    //_page_item_height = 75;
    _page_item_label_height = 25;
    _page_item_value_height = 40;

    uint8_t j = 0;
    strcpy(_main_page[PAGE_WB].label, "White Balance");
    _main_page[PAGE_WB].page_id = PAGE_WB;

    _main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    _main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    _main_page[PAGE_WB].page_item[j].disabled = true;
    j++;
    _main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(_main_page[PAGE_WB].page_item[j].label, "Cancel");
    _main_page[PAGE_WB].page_item[j].highlighted = false;
    _main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    _main_page[PAGE_WB].page_item[j].text_color = _page_item_value_color;
    _main_page[PAGE_WB].page_item[j].background_color = _button_background_color;
    j++;
    _main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(_main_page[PAGE_WB].page_item[j].label, "Add");
    _main_page[PAGE_WB].page_item[j].highlighted = false;
    _main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    _main_page[PAGE_WB].page_item[j].text_color = _page_item_value_color;
    _main_page[PAGE_WB].page_item[j].background_color = _button_background_color;
    j++;
    _main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(_main_page[PAGE_WB].page_item[j].label, "Set");
    _main_page[PAGE_WB].page_item[j].highlighted = false;
    _main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    _main_page[PAGE_WB].page_item[j].text_color = _page_item_value_color;
    _main_page[PAGE_WB].page_item[j].background_color = _button_primary_background_color;

    _main_page[PAGE_WB].page_items_count = j + 1;

    j++;
    _main_page[PAGE_WB].page_item[j].disabled = false;
    strcpy(_main_page[PAGE_WB].page_item[j].label, "Home");
    _main_page[PAGE_WB].page_item[j].highlighted = false;
    _main_page[PAGE_WB].page_item[j].item_type = BUTTON;
    _main_page[PAGE_WB].page_item[j].text_color = _page_item_value_color;
    _main_page[PAGE_WB].page_item[j].background_color = _button_primary_background_color;

    _wb_menu_highlight_index = 0;
}

void wb_page_button_press_handler(ButtonID button_index) {
    if (button_index == P1) {
        _main_page[PAGE_WB].page_item[0].highlighted = true;
    }
    if (button_index == P2) {
        _main_page[PAGE_WB].page_item[1].highlighted = true;
    }
    if (button_index == P3) {
        _main_page[PAGE_WB].page_item[2].highlighted = true;
    }
    if (button_index == P4) {
        _main_page[PAGE_WB].page_item[3].highlighted = true;
    }
    if (button_index == P5) {
        _main_page[PAGE_WB].page_item[4].highlighted = true;
    }
    if (button_index == P6) {
        _main_page[PAGE_WB].page_item[5].highlighted = true;
    }
    if (button_index == P7) {
        _main_page[PAGE_WB].page_item[6].highlighted = true;
    }
    if (button_index == P8) {
        _main_page[PAGE_WB].page_item[7].highlighted = true;
    }
    if (button_index == P9) {
        _main_page[PAGE_WB].page_item[8].highlighted = true;
    }
    if (button_index == P10) {
        _main_page[PAGE_WB].page_item[9].highlighted = true;
    }
    if (button_index == P11) {
        _main_page[PAGE_WB].page_item[10].highlighted = true;
    }
    if (button_index == P12) {

        _main_page[PAGE_WB].page_item[11].highlighted = true;
    }

}

void wb_page_button_release_handler(ButtonID button_index) {
    if (button_index == P1) {
        _main_page[PAGE_WB].page_item[0].highlighted = false;
    }
    if (button_index == P2) {
        _main_page[PAGE_WB].page_item[1].highlighted = false;
    }
    if (button_index == P3) {
        _main_page[PAGE_WB].page_item[2].highlighted = false;
    }
    if (button_index == P4) {
        _main_page[PAGE_WB].page_item[3].highlighted = false;

        navigate_to_page(PAGE_HOME, PUSH_DOWN);
    }
    if (button_index == P5) {
        _main_page[PAGE_WB].page_item[4].highlighted = false;
    }
    if (button_index == P6) {
        // turn off button highlight
        _main_page[PAGE_WB].page_item[5].highlighted = false;

        // set the highlighted white balance
        _white_balance.white_balance_selection_index = _wb_menu_highlight_index;

        // update the new white balance value on the home page
        update_pages();

        // navigate to home page
        navigate_to_page(PAGE_HOME, PUSH_DOWN);
    }
    if (button_index == P7) {

        _main_page[PAGE_WB].page_item[6].highlighted = false;

        navigate_to_page(PAGE_HOME, PUSH_DOWN);
    }
    if (button_index == P8) {

        _main_page[PAGE_WB].page_item[7].highlighted = false;

        navigate_to_page(PAGE_HOME, PUSH_DOWN);
    }
    if (button_index == P9) {
        _main_page[PAGE_WB].page_item[8].highlighted = false;
    }
    if (button_index == P10) {
        _wb_menu_highlight_index--;
        _wb_menu_highlight_index = limit_range(_wb_menu_highlight_index, 0, _white_balance.white_balance_options_count - 1);
        _main_page[PAGE_WB].page_item[9].highlighted = false;
    }
    if (button_index == P11) {
        _main_page[PAGE_WB].page_item[10].highlighted = false;

        navigate_to_page(PAGE_WB_HELP, PUSH_LEFT);
    }
    if (button_index == P12) {
        _wb_menu_highlight_index++;
        _wb_menu_highlight_index = limit_range(_wb_menu_highlight_index, 0, _white_balance.white_balance_options_count - 1);
        _main_page[PAGE_WB].page_item[11].highlighted = false;
    }
    if (button_index == E1) {
        _white_balance.white_balance_selection_index = _wb_menu_highlight_index;

        update_pages();
        navigate_to_page(PAGE_HOME, PUSH_DOWN);
    }
}

void wb_page_knob_handler(ButtonID button_index, int8_t diff) {
    if (button_index == E1_ROT) {
        _wb_menu_highlight_index += diff;
        _wb_menu_highlight_index = limit_range(_wb_menu_highlight_index, 0, _white_balance.white_balance_options_count - 1);
    }
}


#endif /* PAGE_WB_C */