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

// include temp driver
#include "temp.h"

// include utils
#include "../utils/terminal.h"

int main( void ) {
    char buf[20];
    int temp_device = init_temp_device(TEMP_BUS, TEMP_ADR);

    // loop for show temp
    while (1) {

        // read temp from sensor
        float temp_celc = get_temp(temp_device);

        // show on screen an sleep
        show_temp_inline(temp_celc);
        usleep(500000); // 0.5 sec
    }

    return(0);
}