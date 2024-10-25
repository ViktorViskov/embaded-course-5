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

// device info
#define MPC9808_BUS     "/dev/i2c-2"
#define MPC9808_ADR     0x18

// pretty terminal show
void show_temp(float temp) {
    printf("\033[1A\033[K\033[1G"); // return cursor for line up and 1 position
    printf("Temperature: %.2f°C\n", temp);
}

int main( void ) {
    char buf[20];
    int temp_device = init_temp_device(MPC9808_BUS, MPC9808_ADR);

    // loop for show temp
    while (1) {

        // read temp from sensor
        float temp_celc = get_temp(temp_device);

        // show on screen an sleep
        show_temp(temp_celc);
        usleep(500000); // 0.5 sec
    }

    return(0);
}