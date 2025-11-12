//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    src = input;
    dst = output;
    
    while (*src != '\0') {
        if (isspace((unsigned char)*src)) {
            if (in_word) {
                in_word = 0;
                *dst++ = ' ';
            }
            src++;
        } else if (isalnum((unsigned char)*src)) {
            if (!in_word) {
                in_word = 1;
                word_count++;
            }
            *dst++ = *src++;
        } else {
            src++;
        }
    }
    
    if (dst > output && *(dst - 1) == ' ') {
        dst--;
    }
    *dst = '\0';
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    char *start = output;
    char *end = output + strlen(output) - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    printf("Processed string: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    return 0;
}