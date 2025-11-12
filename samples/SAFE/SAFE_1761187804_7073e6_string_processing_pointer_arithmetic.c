//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (dst - output >= sizeof(output) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return EXIT_FAILURE;
            }
            
            if (islower((unsigned char)*src)) {
                *dst = toupper((unsigned char)*src);
            } else {
                *dst = tolower((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            if (dst - output >= sizeof(output) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return EXIT_FAILURE;
            }
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    if (dst - output >= sizeof(output)) {
        fprintf(stderr, "Output buffer overflow\n");
        return EXIT_FAILURE;
    }
    *dst = '\0';
    
    if (dst == output) {
        fprintf(stderr, "No valid characters processed\n");
        return EXIT_FAILURE;
    }
    
    if (printf("%s\n", output) < 0) {
        fprintf(stderr, "Error writing output\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}