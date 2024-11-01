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
#include <mosquitto.h>

// include temp driver
#include "../../src/temp.h"

// include utils
#include "../../utils/time.h"
#include "../../utils/terminal.h"
#include "../../utils/mqtt_config.h"


char ip_address[200];
char topic[200];

void prepare_message(char *buff) {
    char date[20];
    float temp_celc = get_temp();
    get_time_string(date);

    sprintf(buff, "%s\tTemp: %.2lf C", date, temp_celc);
}

int main(int argc, char *argv[])
{
    struct mosquitto *mqtt;
    int version[3];

    // read config
    if (read_config(ip_address, topic) != 0) {
        fprintf(stderr, "Can not read config\n");
        return -1;
    }

    printf("Connect to: %s\n", ip_address);
    printf("Topic: %s\n\n", topic);

    // init mqtt
    mosquitto_lib_init();
    mosquitto_lib_version(&version[0],&version[1],&version[2]);
    printf("Mosquitto library version. %i.%i.%i\n", version[0], version[1], version[2]);

    // create mqtt publisher
    mqtt = mosquitto_new(NULL, true, NULL);
    mosquitto_connect(mqtt, ip_address, 1883, 60);

    // main loop
    while (1) {
        char message[120];
        prepare_message(message);

        int ret = mosquitto_publish(mqtt, NULL, topic, strlen(message), message, 0, false);
        if (ret != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Public err: %s\n", mosquitto_strerror(ret));
            sleep(1);
            continue;
        }

        char status_message[200];
        sprintf(status_message, "Send: %s", message);
        show_text_inline(status_message);
        
        sleep(1);
    }

    // cleanup
    mosquitto_destroy(mqtt);
    mosquitto_lib_cleanup();

    return 0;
}
