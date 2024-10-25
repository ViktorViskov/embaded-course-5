#include <stdio.h>

// float to string conversion
void float_to_string(float number, char *buf) {
    sprintf(buf, "%.2lf", number);
}