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

// device info
#define TEMP_BUS        "/dev/i2c-2"
#define TEMP_ADR        0x18

// MCP9808 registers
#define TEMP_CONFIG_REG 0x01
#define TUPPER_REG      0x02
#define TLOWER_REG      0x03
#define TCRIT_REG       0x04
#define TA_REG          0x05
#define MANID_REG       0x06
#define DEVID_REG       0x07
#define RES_REG         0x08

struct MCP9808_tareg {
    unsigned deg_fraction : 4;  
    unsigned deg : 8;        
    unsigned sign : 1;         
    unsigned t_lower : 1;  
    unsigned t_upper : 1;  
    unsigned t_crit : 1;
};

int temp_device = 0;
int32_t reg32;
uint16_t * const reg16poi = (uint16_t *) &reg32;
struct MCP9808_tareg temp_data;


int16_t _read_from_sensor() {
    reg32 = i2c_smbus_read_word_data(temp_device, TA_REG);
    return bswap_16(reg16poi[0]);
}

// convert bytes from int to struct
void _get_temp_bytes(int raw_data, struct MCP9808_tareg *temp_data) {
    temp_data->sign = (raw_data >> 12) & 0x1;
    temp_data->deg = (raw_data >> 4) & 0xff;
    temp_data->deg_fraction = raw_data & 0xf;

    temp_data->t_lower = (raw_data >> 10) & 0x1;
    temp_data->t_upper = (raw_data >> 11) & 0x1;
    temp_data->t_crit = (raw_data >> 9) & 0x1;

    if (temp_data->sign == 1) {
        temp_data->deg = ~temp_data->deg;
        temp_data->deg_fraction = ~temp_data->deg_fraction;
    }
}

// get celc deg
float _get_celc(struct MCP9808_tareg *temp_data) {
    float fractal = temp_data->deg_fraction / 16.0;
    float temp = temp_data->deg + fractal;

    if (temp_data->sign == 1) {
        temp = -temp;
    }

    return temp;
}

static void _init_temp_device() {
    if (temp_device > 0) {
        return;
    }

    temp_device = open(TEMP_BUS, O_RDWR);
    if (temp_device < 0) { // If error
        fprintf(stderr, "ERROR: opening %s - %s\n", TEMP_BUS, strerror(errno));
        exit(1);
    }

    if (ioctl(temp_device, I2C_SLAVE, TEMP_ADR) == -1 ) { // If error
            fprintf(stderr, "ERROR: setting  address %d on i2c bus %s with ioctl() - %s", TEMP_ADR, TEMP_BUS, strerror(errno) );
            exit(1);
    }
}

float get_temp() {
    _init_temp_device();

    int16_t raw_data = _read_from_sensor();
    _get_temp_bytes(raw_data, &temp_data);
    return _get_celc(&temp_data);
}