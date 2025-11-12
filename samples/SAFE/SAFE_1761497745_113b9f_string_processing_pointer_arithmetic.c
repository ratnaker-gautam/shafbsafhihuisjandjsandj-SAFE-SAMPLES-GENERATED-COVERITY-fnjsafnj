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
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input string\n");
        return EXIT_FAILURE;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
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
    
    printf("Processed string: %s\n", output);
    
    char reversed[MAX_INPUT];
    char *rev_ptr = reversed;
    dst = output + out_len - 1;
    
    while (dst >= output) {
        *rev_ptr = *dst;
        rev_ptr++;
        dst--;
    }
    *rev_ptr = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    size_t word_count = 0;
    char *ptr = output;
    while (*ptr != '\0') {
        while (*ptr == ' ') {
            ptr++;
        }
        if (*ptr != '\0') {
            word_count++;
            while (*ptr != '\0' && *ptr != ' ') {
                ptr++;
            }
        }
    }
    
    printf("Word count: %zu\n", word_count);
    
    return EXIT_SUCCESS;
}