#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include <stdlib.h>
#include <stdbool.h>
#include "usb_descriptors.h"
#include "tusb.h"
#include "bsp/board.h"
#include "Rotary.h"

#define interval_ms  10
#define Button_debounce 20
#define Multimedia_pin 3

typedef struct
{
    bool button ;
    int8_t rotary ;
    bool state;
    bool play_pressed;
    bool play_state ;
}data;
;


void hid_task(void);


int main()
{
    tusb_init();
  ;
    //
    gpio_init(Multimedia_pin) ;
    gpio_set_dir(Multimedia_pin,false);

    setup_buttons(2,Button_debounce) ;
    setup_rotary(0,1) ;

    while(true)
    {

        tud_task(); // tinyusb device task
        hid_task();
        sleep_ms(10);
 }
    return 0;
}
static void send_hid_report(uint8_t report_id, data list)
{
    // with static at compile time the variable will become constant
   static int8_t previous_counter ;
    static bool report_sent = 0  ;


    // skip if hid is not ready yet
    if ( !tud_hid_ready() ) return;
            // use to avoid send multiple consecutive zero report

    if(report_id == REPORT_ID_CONSUMER_CONTROL) {

        if (list.rotary > previous_counter && report_sent == 0) {
            report_sent = 1;
            const uint16_t volume_up = HID_USAGE_CONSUMER_VOLUME_INCREMENT;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_up, 2);
        }
        if (list.rotary < previous_counter && report_sent == 0) {
            report_sent = 1;
            const uint16_t volume_down = HID_USAGE_CONSUMER_VOLUME_DECREMENT;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &volume_down, 2);
        }
        if (list.state == 1  && report_sent == 0) {
            report_sent = 1;
            const uint16_t Mute = HID_USAGE_CONSUMER_MUTE;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &Mute, 2);

        }
        if ( list.play_pressed == 1   && report_sent == 0  ) {
            report_sent = 1;
            const uint16_t Play = HID_USAGE_CONSUMER_PLAY_PAUSE;
            tud_hid_report(REPORT_ID_CONSUMER_CONTROL, &Play, 2);

        }
        else if(list.play_pressed == 0 )
        {
          uint16_t empty_key = 0 ;
         tud_hid_report(REPORT_ID_CONSUMER_CONTROL,&empty_key,2) ;
            report_sent = false;

        }

            // update the previous state to get the new data
        previous_counter = list.rotary ;


    }


}

// Every 10ms, we will sent 1 report for each HID profile
void hid_task(void)
{


    uint32_t const btn = board_button_read();

    if ( tud_suspended() && btn )
    {
        tud_remote_wakeup();
    }
    data to_send ;
    static int8_t counter ;
    static bool store_prev ;
    to_send.play_pressed = gpio_get(Multimedia_pin);
    to_send.button =   but_stat() ;
    get_rotary_status(&counter) ;


            if (to_send.button == 1 && store_prev == 0   )
            {
                store_prev = 1 ;
                to_send.state = !to_send.state;
                sleep_ms(4) ;
            }
        else if(to_send.button == 0 && store_prev == 1  )
            {
                store_prev = 0;
                sleep_ms(4);
            }


            to_send.rotary = counter;
            send_hid_report(REPORT_ID_CONSUMER_CONTROL, to_send);

}



void tud_hid_report_complete_cb(uint8_t instance, uint8_t const* report, uint8_t len)
{
    (void) instance;
    (void) len;

    uint8_t next_report_id = report[0] + 2;

    if (next_report_id < REPORT_ID_COUNT)
    {
        data empty ;
        empty.button = board_button_read() ;
        send_hid_report(next_report_id, empty);
    }
}


uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen)
{
    // TODO not Implemented
    (void) instance;
    (void) report_id;
    (void) report_type;
    (void) buffer;
    (void) reqlen;
    return 0;
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize)
{
    (void)instance ;
    (void)report_id ;
    (void)report_type ;
    (void)buffer ;
    (void)bufsize ;
}


