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

    int lcd_device = init_lcd_device();
    int rgb_device = init_rgb_device();
    int temp_device = init_temp_device();

    clear_lcd(lcd_device);
    enable_rgb(rgb_device);
    reset_rgb(rgb_device);

    // loop for show temp
    while (1) {
        get_time_string(buf_time);

        // read temp from sensor
        float temp_celc = get_temp(temp_device);

        // show on screen an sleep
        float_to_string(temp_celc, buf_temp);
        display_string_first_line(lcd_device, buf_time);
        display_string_second_line(lcd_device, buf_temp);

        // set red led if temp > 30
        if (temp_celc >= 30.0 ){
            set_red(rgb_device, 255);
        } else {
            set_red(rgb_device, 0);
        }

        // set green led if temp < 30 and > 5
        if (temp_celc < 30.0 && temp_celc > 5.0 ){
            set_green(rgb_device, 255);
        } else {
            set_green(rgb_device, 0);
        }

        // set blue led if temp < 5
        if (temp_celc <= 5.0 ){
            set_blue(rgb_device, 255);
        } else {
            set_blue(rgb_device, 0);
        }
        usleep(500000); // 0.5 sec
    }

    return(0);
}