#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


#define MTQQ_DEFAULT_BROKER "localhost"
#define MTQQ_DEFAULT_TOPIC "viktor_v/house_temp/celc"
#define CONFIG_FILE "./config.txt"
#define MAX_LINE_LENGTH 200


int _create_default_config(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    // Перевіряємо, чи файл існує
    if (file != NULL) {
        fclose(file);
        return 0;
    }
    
    file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Can not create config file\n");
        return -1;
    }

    fprintf(file, "%s\n", MTQQ_DEFAULT_BROKER);
    fprintf(file, "%s\n", MTQQ_DEFAULT_TOPIC);

    fclose(file);
    printf("Config file created\n");
    return 0;
}

int read_config(char *ip_address, char *topic) {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
        // fprintf(stderr, "Can not open config file: '%s'\n", CONFIG_FILE);
        printf("Can not open config file: '%s'. Creating new one ...\n", CONFIG_FILE);

        // create default config
        if (_create_default_config(CONFIG_FILE) != 0) {
            return -1;
        }
        file = fopen(CONFIG_FILE, "r");
    }

    // read ip address
    if (fgets(ip_address, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Can not read IP from config\n");
        fclose(file);
        return -1;
    }
    
    // read topic
    if (fgets(topic, MAX_LINE_LENGTH, file) == NULL) {
        fprintf(stderr, "Can not read topic from config\n");
        fclose(file);
        return -1;
    }

    fclose(file);

    // replace new line with null terminator
    ip_address[strcspn(ip_address, "\n")] = '\0';
    topic[strcspn(topic, "\n")] = '\0';

    return 0;
}