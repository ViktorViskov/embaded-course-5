//Author: heth@mercantec.dk
//Date..: 19. nov 2022
//Ver...: 0.2 beta
//
//Modification:
//   01/04/2022 - Simplified: Only one mosquitto instance used in main()
//
//INFORMATION
// Install mosquitto and mosquitto development files with:
//    sudo apt -y install mosquitto mosquitto-dev mosquitto-clients
//Mosquitto docementation:
//   - https://mosquitto.org/documentation/
//   - https://github.com/eclipse/mosquitto (Se under examples)
//Compile with : gcc mqtt_ex1.c -lmosquitto -o mqtt_ex1
#include <stdio.h>
#include <mosquitto.h>
#include <string.h>
#include <stdlib.h>
int number;

#define SUCCES 0
void _on_connect(struct mosquitto *mosq, void *obj, int result)
{
    int rc = MOSQ_ERR_SUCCESS;

    if(result == SUCCES){
        mosquitto_subscribe(mosq, NULL, "henrik/cputemp/fahrenheit", 0);  // NOTE: Change to published topic
    }else{
        fprintf(stderr, "%s\n", mosquitto_connack_string(result));
    }
}

void _on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{
    struct mosquitto *mosq2 = (struct mosquitto *)obj;
        char *txtpoi, *txtpoi2;
        printf("struct mosquitto_message contains:\n");
        printf("  mid (int).............: %i\n", message->mid );
        printf("  topic (* char)........: %s\n", message->topic );
        printf("  payload (* void)......: %p", message->payload );
        txtpoi = malloc(message->payloadlen + 1);
        if ( txtpoi == 0 ) {
                fprintf( stderr, "Malloc error\n");
        } else {
                strncpy(txtpoi, message->payload, message->payloadlen);
                txtpoi[message->payloadlen] = 0;
                printf("  Message: [%s]\n", txtpoi);
        }
        printf("  payloadlen (int)......: %i\n", message->payloadlen );
        printf("  qos (int).............: %i\n", message->qos );
        printf("  retain (int)..........: %i\n", message->retain );

        txtpoi2 = malloc( message->payloadlen + 20);
        sprintf(txtpoi2, "#%i:%s", number, txtpoi);
        number++;

        mosquitto_publish(mosq, NULL, "user/DIT_NAVN_HER_ELLER_WHATEVER/english", strlen(txtpoi2), txtpoi2,  message->qos, message->retain);
        free(txtpoi2);
        free(txtpoi);
}

int main(int argc, char *argv[])
{
    struct mosquitto *mosq1;
    int version[3];
    number = 1; // Init mesage number

    mosquitto_lib_init();
    mosquitto_lib_version(&version[0],&version[1],&version[2]);
    printf("Mosquitto library version. %i.%i.%i\n", version[0], version[1], version[2]);

    mosq1 = mosquitto_new(NULL, true, NULL);

    mosquitto_connect_callback_set(mosq1, _on_connect);
    mosquitto_message_callback_set(mosq1, _on_message);

    mosquitto_connect(mosq1, "93.166.84.21", 1883, 60);  // NOTE:  Replace localhost with IP address of broker


    mosquitto_loop_forever(mosq1, -1, 1);

    mosquitto_destroy(mosq1);

    mosquitto_lib_cleanup();

    return 0;
}
