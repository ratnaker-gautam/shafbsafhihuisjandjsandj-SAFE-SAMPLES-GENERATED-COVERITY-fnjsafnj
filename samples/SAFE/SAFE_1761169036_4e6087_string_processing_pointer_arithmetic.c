//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main(void) {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (len >= MAX_INPUT) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
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
        }
        src++;
    }
    
    *dst = '\0';
    
    size_t out_len = dst - output;
    if (out_len == 0) {
        fprintf(stderr, "No valid characters found\n");
        return 1;
    }
    
    printf("Processed string: ");
    char *print_ptr = output;
    while (*print_ptr) {
        putchar(*print_ptr);
        print_ptr++;
    }
    putchar('\n');
    
    return 0;
}