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
    int word_count = 0;
    int in_word = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    src = input;
    dst = output;
    char *end = input + input_len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *dst = *src;
            dst++;
        } else if (isspace((unsigned char)*src)) {
            if (in_word) {
                in_word = 0;
                *dst = ' ';
                dst++;
            }
        }
        src++;
    }
    
    if (dst > output && *(dst - 1) == ' ') {
        dst--;
    }
    *dst = '\0';
    
    size_t output_len = dst - output;
    if (output_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Output buffer overflow\n");
        return EXIT_FAILURE;
    }
    
    printf("Processed text: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_INPUT_LEN];
    char *rev_ptr = reversed;
    dst = output + output_len - 1;
    
    while (dst >= output) {
        *rev_ptr = *dst;
        rev_ptr++;
        dst--;
    }
    *rev_ptr = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return EXIT_SUCCESS;
}