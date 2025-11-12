//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end && *src != '\0') {
        if (isalpha((unsigned char)*src)) {
            if (islower((unsigned char)*src)) {
                *dst = (char)toupper((unsigned char)*src);
            } else {
                *dst = (char)tolower((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    if (dst - output >= MAX_INPUT_LEN) {
        fprintf(stderr, "Output buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    *dst = '\0';
    
    if (fputs(output, stdout) == EOF) {
        fprintf(stderr, "Error writing output\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}