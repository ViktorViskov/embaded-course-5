#include <stdio.h>

// pretty terminal show
void show_temp_inline(float temp) {
    printf("\033[1A\033[K\033[1G"); // return cursor for line up and 1 position
    printf("Temperature: %.2f°C\n", temp);
}

// pretty terminal show text
void show_text_inline(char *text) {
    printf("\033[1A\033[K\033[1G"); // return cursor for line up and 1 position
    printf("%s\n", text);
}