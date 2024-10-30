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

//Device specific information
#define RGB_BUS        "/dev/i2c-2"
#define RGB_ADR         0x62

int rgb_device = 0;

// RGB values init
char red, green, blue = 0;

static void _init_rgb_device() {
    if (rgb_device) {
        return;
    }

    rgb_device = open(RGB_BUS, O_RDWR);
    if (rgb_device < 0) { // If error
        fprintf(stderr, "ERROR: opening %s - %s\n", RGB_BUS, strerror(errno));
        exit(1);
    }

    if (ioctl(rgb_device, I2C_SLAVE, RGB_ADR) == -1 ) { // If error
            fprintf(stderr, "ERROR: setting  address %d on i2c bus %s with ioctl() - %s", RGB_ADR, RGB_BUS, strerror(errno) );
            exit(1);
    }
}

void enable_rgb() {
    _init_rgb_device();
    i2c_smbus_write_byte_data(rgb_device, 0, 0);
    i2c_smbus_write_byte_data(rgb_device, 1, 0);
    i2c_smbus_write_byte_data(rgb_device, 0x08, 0xff);
}

void disable_rgb() {
    _init_rgb_device();
    i2c_smbus_write_byte_data(rgb_device, 0, 0);
    i2c_smbus_write_byte_data(rgb_device, 1, 0);
    i2c_smbus_write_byte_data(rgb_device, 0x08, 0);
}

void set_red(char red) {
    _init_rgb_device();
    i2c_smbus_write_byte_data(rgb_device, 4, red);
    red = red;
}

void set_green(char green) {
    _init_rgb_device();
    i2c_smbus_write_byte_data(rgb_device, 3, green);
    green = green;
}

void set_blue(char blue) {
    _init_rgb_device();
    i2c_smbus_write_byte_data(rgb_device, 2, blue);
    blue = blue;
}

void set_rgb(char red, char green, char blue) {
    set_red(red);
    set_green(green);
    set_blue(blue);
}

void reset_rgb() {
    set_rgb(0, 0, 0);
}

char get_rgb_red() {
    return red;
}

char get_rgb_green() {
    return green;
}

char get_rgb_blue() {
    return blue;
}