#include <stdio.h>
#include <time.h>


void get_time_string(char *buf) {
    time_t now = time(NULL);
    struct tm *time_ptr = localtime(&now);

    strftime(buf, 20, "%d-%m-%Y %H:%M:%S", time_ptr);
} 