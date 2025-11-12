//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    char *src = input;
    char *dst = output;
    char *end = output + sizeof(output) - 1;
    
    while (*src != '\0' && dst < end) {
        if (isalpha((unsigned char)*src)) {
            if (isupper((unsigned char)*src)) {
                *dst = tolower((unsigned char)*src);
            } else {
                *dst = toupper((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        } else if (isspace((unsigned char)*src)) {
            if (dst > output && *(dst - 1) != ' ') {
                *dst = ' ';
                dst++;
            }
        }
        src++;
    }
    
    if (dst == output) {
        *dst = '\0';
    } else {
        if (*(dst - 1) == ' ') {
            dst--;
        }
        *dst = '\0';
    }
    
    if (dst >= end) {
        fprintf(stderr, "Output buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed string: %s\n", output);
    
    return EXIT_SUCCESS;
}