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
    if (len == 0 || len >= MAX_INPUT) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            *dst = toupper((unsigned char)*src);
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        } else if (isspace((unsigned char)*src)) {
            *dst = ' ';
            dst++;
        }
        src++;
    }
    *dst = '\0';
    
    size_t out_len = dst - output;
    if (out_len == 0) {
        fprintf(stderr, "No valid characters found\n");
        return EXIT_FAILURE;
    }
    
    if (fwrite(output, sizeof(char), out_len, stdout) != out_len) {
        fprintf(stderr, "Error writing output\n");
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}