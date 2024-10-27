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
#include "../src/lcd.h"

// include utils
#include "../utils/converting.h"


int main( void ) {
    char buf[16];
    int lcd_device = init_lcd_device();
    float temp = -12.34;
    float_to_string(temp, buf);

    clear_lcd(lcd_device);

    display_string_first_line(lcd_device, buf);
    display_string_second_line(lcd_device, "Hello world");

    return(0);
}