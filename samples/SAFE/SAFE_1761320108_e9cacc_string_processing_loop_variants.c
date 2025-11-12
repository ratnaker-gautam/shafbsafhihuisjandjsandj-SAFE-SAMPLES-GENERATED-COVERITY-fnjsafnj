//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char output[MAX_LENGTH];
    int i, j, len;
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    
    printf("Alphanumeric characters: ");
    i = 0;
    while (output[i] != '\0') {
        putchar(output[i]);
        i++;
    }
    putchar('\n');
    
    printf("Reversed: ");
    int k = strlen(output) - 1;
    do {
        if (k >= 0) {
            putchar(output[k]);
        }
        k--;
    } while (k >= 0);
    putchar('\n');
    
    printf("Uppercase: ");
    char *ptr = output;
    while (*ptr) {
        putchar(toupper((unsigned char)*ptr));
        ptr++;
    }
    putchar('\n');
    
    int digit_count = 0;
    for (i = 0; output[i] != '\0'; i++) {
        if (isdigit((unsigned char)output[i])) {
            digit_count++;
        }
    }
    printf("Digit count: %d\n", digit_count);
    
    return 0;
}