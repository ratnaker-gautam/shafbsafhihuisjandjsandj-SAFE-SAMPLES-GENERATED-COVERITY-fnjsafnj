//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    char *src, *dst;
    int word_count = 0;
    int in_word = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
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
        } else {
            if (in_word && word_count > 1) {
                *dst = ' ';
                dst++;
            }
            in_word = 0;
        }
        src++;
    }
    
    if (dst > output && *(dst - 1) == ' ') {
        dst--;
    }
    *dst = '\0';
    
    if (dst - output >= sizeof(output)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    printf("Processed text: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    return 0;
}