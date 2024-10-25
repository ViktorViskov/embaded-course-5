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
#define LCD_BUS        "/dev/i2c-2"
#define LCD_ADR         0x3e

// LCD commands
#define TWO_LINE_MODE   0x28
#define CURSOR_MODE     0x0D
#define CLEAR           0x01
#define ENTRY           0x06
#define UKNOWN          0x02
#define LINE_ONE        0x80
#define LINE_TWO        0xC0

// LCD memory addresses
#define CONFIG_REG      0x00
#define LCD_REG         0x40


static int _init_lcd_file(char *bus, unsigned int address) {
    int file;

    file = open(bus, O_RDWR);
    if (file < 0) { // If error
        fprintf(stderr, "ERROR: opening %s - %s\n", bus, strerror(errno));
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, address) == -1 ) { // If error
            fprintf(stderr, "ERROR: setting  address %d on i2c bus %s with ioctl() - %s", address, bus, strerror(errno) );
            exit(1);
    }
    return(file);
}

static void _init_lcd_device(int file) {
    i2c_smbus_write_byte_data(file, CONFIG_REG, TWO_LINE_MODE);
    i2c_smbus_write_byte_data(file, CONFIG_REG, CURSOR_MODE);
}

// init screen file and device
int init_lcd(char *bus, unsigned int address) {
    int file;

    file = _init_lcd_file(bus, address);
    _init_lcd_device(file);
    return(file);
}

void clear_lcd(int device) {
    i2c_smbus_write_byte_data(device, CONFIG_REG, 0x01);
    i2c_smbus_write_byte_data(device, CONFIG_REG, 0x06);
    i2c_smbus_write_byte_data(device, CONFIG_REG, 0x02);
}

void display_string_first_line(int device, char *str) {
    int reg32;

    i2c_smbus_write_byte_data(device, CONFIG_REG, LINE_ONE);
    while(*str != 0) {
        reg32 = i2c_smbus_write_byte_data(device, LCD_REG, *str);
        str++;
        if ( reg32 < 0 ) {
            fprintf(stderr, "ERROR: Write failed in strout\n");
            exit(1);
        }
    }
}

void display_string_second_line(int device, char *str) {
    int reg32;

    i2c_smbus_write_byte_data(device, CONFIG_REG, LINE_TWO);
    while(*str != 0) {
        reg32 = i2c_smbus_write_byte_data(device, LCD_REG, *str);
        str++;
        if ( reg32 < 0 ) {
            fprintf(stderr, "ERROR: Write failed in strout\n");
            exit(1);
        }
    }
}