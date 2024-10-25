#include <stdio.h>

// include drivers
#include "../lcd/lcd.h"
#include "../temp/temp.h"

// include utils
#include "../utils/converting.h"


int main( void ) {
    char buf[16];
    int lcd_device = init_lcd_device(LCD_BUS, LCD_ADR);
    int temp_device = init_temp_device(TEMP_BUS, TEMP_ADR);

    clear_lcd(lcd_device);

    // loop for show temp
    while (1) {

        // read temp from sensor
        float temp_celc = get_temp(temp_device);

        // show on screen an sleep
        float_to_string(temp_celc, buf);
        display_string_first_line(lcd_device, buf);
        display_string_second_line(lcd_device, "Hello World");
        usleep(500000); // 0.5 sec
    }

    return(0);
}