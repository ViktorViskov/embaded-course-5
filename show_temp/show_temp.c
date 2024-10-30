#include <stdio.h>

// include drivers
#include "../src/lcd.h"
#include "../src/temp.h"
#include "../src/rbg.h"
#include "../utils/time.h"

// include utils
#include "../utils/converting.h"


int main( void ) {
    char buf_temp[16];
    char buf_time[20];

    clear_lcd();
    enable_rgb();
    reset_rgb();

    // loop for show temp
    while (1) {
        get_time_string(buf_time);

        // read temp from sensor
        float temp_celc = get_temp();

        // show on screen an sleep
        float_to_string(temp_celc, buf_temp);
        display_string_first_line(buf_time);
        display_string_second_line(buf_temp);

        // set red led if temp > 30
        if (temp_celc >= 30.0 ){
            set_red(255);
        } else {
            set_red(0);
        }

        // set green led if temp < 30 and > 5
        if (temp_celc < 30.0 && temp_celc > 5.0 ){
            set_green(255);
        } else {
            set_green(0);
        }

        // set blue led if temp < 5
        if (temp_celc <= 5.0 ){
            set_blue(255);
        } else {
            set_blue(0);
        }
        usleep(500000); // 0.5 sec
    }

    return(0);
}