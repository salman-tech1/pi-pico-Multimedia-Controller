//
// Created by Salman on 3/31/2023.
//
#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>
#include "Rotary.h"
#include "bsp/board.h"

// struct defined global
button_settings data ;
rotary_settings rot_data;

volatile bool cond_CW = 0 , cond_CCW = 0  ;
int8_t counter = 0 ;
extern void setup_rotary(uint8_t dt_pin , uint8_t clk_pin)
{
    rot_data.data_pin = dt_pin ;
    rot_data.clok_pin = clk_pin ;


    gpio_init(  rot_data.data_pin ) ;
    gpio_init(rot_data.clok_pin) ;
    // set the direction to input
    gpio_set_dir(rot_data.data_pin,false) ;
    gpio_set_dir(rot_data.clok_pin,false) ;

    // Set the slew rate fast which indicate that the voltage ramp changing fast
    // and reduce the Electctro magnetic emission
    gpio_set_slew_rate(rot_data.data_pin,GPIO_SLEW_RATE_FAST) ;
    gpio_set_slew_rate(rot_data.clok_pin,GPIO_SLEW_RATE_FAST) ;

    // to get the previous status of dt_pin and clk_pin
    rot_data.prev_data_status = gpio_get(rot_data.data_pin);
    rot_data.prev_clk_status = gpio_get(rot_data.clok_pin);

    // storing this data in last status to compare to new data
    // shift one bit to the left so that left space for other bit
    rot_data.last_rotary_status = rot_data.prev_data_status << 1 | rot_data.prev_clk_status ;



}
extern void get_rotary_status(int8_t *ptr )
{
    // getting current data
    rot_data.current_data_status = gpio_get(rot_data.data_pin) ;
    rot_data.current_clk_status = gpio_get(rot_data.clok_pin) ;

    // merge bits to compare to the last status
    rot_data.current_rotary_status = rot_data.current_data_status << 1 | rot_data.current_clk_status ;



    // merging to get the new data
    rot_data.change_data = (rot_data.last_rotary_status << 2 ) | rot_data.current_rotary_status ;

 if(rot_data.change_data == 0b1110 || rot_data.change_data == 0b1101 && rot_data.current_rotary_status  != rot_data.last_rotary_status) {


     switch (rot_data.change_data) {
         // CW = 0b1110
         case CW :
             if(rot_data.change_data == CW && cond_CW == false  )
             {

                 cond_CW = true ;
                 if(counter >= 255 )counter = 255 ;
                 counter++;
             }
             break;
         case CCW :
             if(rot_data.change_data == CCW && cond_CCW == false )
             {
                 if(counter <= -255 )counter = -255 ;
                 cond_CCW = true ;
                 counter--;
             }
             break;


     }



 }
 // reset the timer

    *ptr = counter ;
    cond_CCW = false ;
    cond_CW = false ;
    rot_data.last_rotary_status = rot_data.current_rotary_status ;

}
extern void setup_buttons(uint8_t butt_pin , uint32_t set_debounce )
{

    data.pin = butt_pin ;

    gpio_init(data.pin) ;

    gpio_set_dir(data.pin,false) ;
    gpio_pull_up(data.pin) ;
        // make sure that your debounce time is greater than 0
        if(set_debounce > 0 )data.debounce = set_debounce ;
        else data.debounce = 0 ;

    // this will be the previous_pressed state of button pressed or ot pressed
    data.previous_pressed = gpio_get(data.pin) ;

}

// to get the button press
extern int8_t but_stat()
{

    // current switch status
    data.button_state = gpio_get(data.pin) ;


    if(   data.button_state != data.previous_pressed && data.current_pressed == 0 && board_millis() - data.Timer >=  data.debounce )
    {
        data.Timer = board_millis() ;
        data.current_pressed = 1 ;
        data.previous_pressed = data.button_state ;

        return 1 ;

    }
     else if(  data.button_state != data.previous_pressed && data.current_pressed == 1 && board_millis() - data.Timer >=  data.debounce  )
    {

        data.Timer = board_millis() ;
        data.current_pressed = 0 ;
        data.previous_pressed =  data.button_state;
        return -1 ;
    }
     return 0 ;
}


