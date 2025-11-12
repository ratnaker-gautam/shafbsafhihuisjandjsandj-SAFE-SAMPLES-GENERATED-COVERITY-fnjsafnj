//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    char *src, *dst;
    size_t len;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    src = input;
    dst = output;
    
    while (*src != '\0' && *src != '\n') {
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
        
        if ((size_t)(dst - output) >= sizeof(output) - 1) {
            break;
        }
    }
    
    *dst = '\0';
    
    if (strlen(output) == 0) {
        fprintf(stderr, "No valid characters found\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed: %s\n", output);
    return EXIT_SUCCESS;
}