//
// Created by Salman on 4/1/2023.
//

#ifndef ROTARY_CHECK_ROTARY_H
#define ROTARY_CHECK_ROTARY_H

#endif //ROTARY_CHECK_ROTARY_H
#include <stdbool.h>
#include <stdio.h>
#include "pico/stdlib.h"
#ifndef CW
#define CW 0b1110
#endif
#ifndef CCW
#define CCW 0b1101
#endif

#define rotary_debounce 10



 typedef struct
{
    uint8_t pin ;
    uint8_t previous_pressed ;
    bool button_state ;
    bool current_pressed  ;
    uint32_t debounce ;
    uint32_t Timer ;

}button_settings;

typedef struct
{
    uint8_t data_pin ;
    uint8_t clok_pin ;
    bool prev_clk_status ;
    bool prev_data_status ;
    volatile  bool current_clk_status ;
    volatile bool current_data_status ;

    uint8_t last_rotary_status ;
    uint8_t current_rotary_status ;
    uint8_t change_data;

    uint8_t rotary_del ;
}rotary_settings;

extern void setup_buttons(uint8_t butt_pin , uint32_t set_debounce );
extern void setup_rotary(uint8_t dt_pin , uint8_t clk_pin) ;
extern void get_rotary_status(int8_t *ptr );
extern int8_t but_stat();