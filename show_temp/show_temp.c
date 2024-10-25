#include <stdio.h>

// include drivers
#include "../lcd/lcd.h"
#include "../temp/temp.h"
#include "../lcd/rbg.h"

// include utils
#include "../utils/converting.h"


int main( void ) {
    char buf[16];
    int lcd_device = init_lcd_device(LCD_BUS, LCD_ADR);
    int rgb_device = init_rgb_device(RGB_BUS, RGB_ADR);
    int temp_device = init_temp_device(TEMP_BUS, TEMP_ADR);

    clear_lcd(lcd_device);
    enable_rgb(rgb_device);
    reset_rgb(rgb_device);

    // loop for show temp
    while (1) {

        // read temp from sensor
        float temp_celc = get_temp(temp_device);

        // show on screen an sleep
        float_to_string(temp_celc, buf);
        display_string_first_line(lcd_device, buf);
        display_string_second_line(lcd_device, "Hello World");

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