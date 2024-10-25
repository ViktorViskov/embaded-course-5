#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
#include <byteswap.h>

// include lcd driver
#include "lcd.h"

// float to string
void _float_to_string(float f, char *buf) {
    int a = f;
    int b = (f - a) * 100;
    sprintf(buf, "%d.%d", a, b);
}


int main( void ) {
    char buf[16];
    int lcd_device = init_lcd_device(LCD_BUS, LCD_ADR);
    float temp = 12.34;
    _float_to_string(temp, buf);

    clear_lcd(lcd_device);

    display_string_first_line(lcd_device, buf);
    display_string_second_line(lcd_device, "Hello world");

    return(0);
}