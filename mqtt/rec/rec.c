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
#include "../../src/lcd.h"

// include utils
#include "../../utils/mqtt_config.h"
#include "../../utils/terminal.h"


#define MTQQ_BROKER "localhost"
#define MTQQ_TOPIC "viktor_v/house_temp/celc"


char ip_address[200];
char topic[200];

void _on_connect(struct mosquitto *mosq, void *obj, int result)
{
    if(result != MOSQ_ERR_SUCCESS){
        fprintf(stderr, "%s\n", mosquitto_connack_string(result));
        return;
    }

    mosquitto_subscribe(mosq, NULL, topic, 0);
}

void _on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    // show on terminal
    show_text_inline(message->payload);

    // show on lcd
    clear_lcd();
    
    // split message by \t
    char *date = strtok(message->payload, "\t");
    char *temp = strtok(NULL, "\t");

    clear_lcd();
    display_string_first_line(date);
    display_string_second_line(temp);
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

    // create mqtt receiver
    mqtt = mosquitto_new(NULL, true, NULL);
    mosquitto_connect_callback_set(mqtt, _on_connect);
    mosquitto_message_callback_set(mqtt, _on_message);
    mosquitto_connect(mqtt, ip_address, 1883, 60);

    // main loop
    mosquitto_loop_forever(mqtt, -1, 1);

    // cleanup
    mosquitto_destroy(mqtt);
    mosquitto_lib_cleanup();

    return 0;
}
