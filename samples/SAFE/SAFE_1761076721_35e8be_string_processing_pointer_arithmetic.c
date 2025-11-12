//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (islower((unsigned char)*src)) {
                *dst = toupper((unsigned char)*src);
            } else {
                *dst = tolower((unsigned char)*src);
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
    
    if (out_len >= MAX_INPUT) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    printf("Processed string: %s\n", output);
    printf("Original length: %zu, Processed length: %zu\n", len, out_len);
    
    return 0;
}