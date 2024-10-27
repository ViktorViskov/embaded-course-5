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

// RGB values init
char red, green, blue = 0;

static int init_rgb_device() {
    int device = open(RGB_BUS, O_RDWR);
    if (device < 0) { // If error
        fprintf(stderr, "ERROR: opening %s - %s\n", RGB_BUS, strerror(errno));
        exit(1);
    }

    if (ioctl(device, I2C_SLAVE, RGB_ADR) == -1 ) { // If error
            fprintf(stderr, "ERROR: setting  address %d on i2c bus %s with ioctl() - %s", RGB_ADR, RGB_BUS, strerror(errno) );
            exit(1);
    }
    return(device);
}

void enable_rgb(int device) {
    i2c_smbus_write_byte_data(device, 0, 0);
    i2c_smbus_write_byte_data(device, 1, 0);
    i2c_smbus_write_byte_data(device, 0x08, 0xff);
}

void disable_rgb(int device) {
    i2c_smbus_write_byte_data(device, 0, 0);
    i2c_smbus_write_byte_data(device, 1, 0);
    i2c_smbus_write_byte_data(device, 0x08, 0);
}

void set_red(int device, char red) {
    i2c_smbus_write_byte_data(device, 4, red);
    red = red;
}

void set_green(int device, char green) {
    i2c_smbus_write_byte_data(device, 3, green);
    green = green;
}

void set_blue(int device, char blue) {
    i2c_smbus_write_byte_data(device, 2, blue);
    blue = blue;
}

void set_rgb(int device, char red, char green, char blue) {
    set_red(device, red);
    set_green(device, green);
    set_blue(device, blue);
}

void reset_rgb(int device) {
    set_rgb(device, 0, 0, 0);
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