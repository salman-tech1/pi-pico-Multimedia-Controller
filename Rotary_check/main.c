//
// Created by Salman on 4/1/2023.
//
#include <stdio.h>
#include "pico/stdlib.h"
#include "Rotary.h"

#ifndef MAX_TIME_COUNT
#define MAX_TIME_COUNT 10
#endif

int8_t count = 0  ;

int main()
{
    stdio_init_all() ;
    // this will setup the pin as an input
    //setup_buttons(2,MAX_TIME_COUNT) ;
    setup_rotary(0,1) ;
    setup_buttons(2,50) ;


    while(1)
    {
        int8_t button  = but_stat() ;
     //   int8_t button_read = but_stat() ;
        get_rotary_status(&count) ;


        if(button == 1 )
        printf("button Pressed %d \n",button) ;
        else if(button == -1)
            printf("button released %d \n",button) ;

    }
}