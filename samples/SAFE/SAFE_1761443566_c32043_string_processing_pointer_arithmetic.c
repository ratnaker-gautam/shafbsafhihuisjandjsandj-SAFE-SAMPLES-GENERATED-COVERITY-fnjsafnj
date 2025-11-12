//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + input_len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (dst - output >= MAX_INPUT_LEN - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            if (islower((unsigned char)*src)) {
                *dst = toupper((unsigned char)*src);
            } else {
                *dst = tolower((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            if (dst - output >= MAX_INPUT_LEN - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    if (dst - output >= MAX_INPUT_LEN) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    *dst = '\0';
    
    size_t output_len = dst - output;
    if (output_len == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    printf("Processed string: %s\n", output);
    printf("Original length: %zu, Processed length: %zu\n", input_len, output_len);
    
    char *rev_ptr = output + output_len - 1;
    printf("Reversed: ");
    while (rev_ptr >= output) {
        putchar(*rev_ptr);
        rev_ptr--;
    }
    putchar('\n');
    
    return 0;
}