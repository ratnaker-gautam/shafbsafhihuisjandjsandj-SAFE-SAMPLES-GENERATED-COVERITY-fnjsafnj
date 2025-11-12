//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    char *src, *dst;
    int word_count = 0;
    int in_word = 0;
    
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
        printf("Empty input\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    src = input;
    dst = output;
    
    while (*src != '\0') {
        if (isalpha((unsigned char)*src)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *dst = toupper((unsigned char)*src);
            dst++;
        } else if (isspace((unsigned char)*src)) {
            if (in_word) {
                in_word = 0;
                *dst = ' ';
                dst++;
            }
        } else {
            *dst = *src;
            dst++;
        }
        src++;
        
        if ((size_t)(dst - output) >= MAX_LEN - 1) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
    }
    
    *dst = '\0';
    
    if (dst > output && *(dst - 1) == ' ') {
        *(dst - 1) = '\0';
    }
    
    printf("Processed string: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    return 0;
}