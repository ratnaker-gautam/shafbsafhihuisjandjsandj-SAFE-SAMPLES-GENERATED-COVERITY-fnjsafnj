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
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    src = input;
    dst = output;
    char *output_end = output + MAX_INPUT_LEN - 1;
    
    while (*src != '\0' && dst < output_end) {
        if (isspace((unsigned char)*src)) {
            if (in_word) {
                word_count++;
                in_word = 0;
            }
            *dst++ = *src++;
        } else if (isalnum((unsigned char)*src)) {
            in_word = 1;
            *dst++ = *src++;
        } else {
            src++;
        }
    }
    
    if (in_word) {
        word_count++;
    }
    
    if (dst >= output_end) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    *dst = '\0';
    
    printf("Processed text: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_INPUT_LEN];
    char *rev_ptr = reversed;
    char *word_start = output;
    char *word_end = output;
    
    while (*word_end != '\0' && rev_ptr < reversed + MAX_INPUT_LEN - 1) {
        if (!isspace((unsigned char)*word_end)) {
            word_end++;
            continue;
        }
        
        char *temp = word_end - 1;
        while (temp >= word_start && rev_ptr < reversed + MAX_INPUT_LEN - 1) {
            *rev_ptr++ = *temp--;
        }
        
        if (rev_ptr < reversed + MAX_INPUT_LEN - 1) {
            *rev_ptr++ = ' ';
        }
        
        word_start = word_end + 1;
        word_end = word_start;
    }
    
    if (word_start < word_end) {
        char *temp = word_end - 1;
        while (temp >= word_start && rev_ptr < reversed + MAX_INPUT_LEN - 1) {
            *rev_ptr++ = *temp--;
        }
    }
    
    if (rev_ptr >= reversed + MAX_INPUT_LEN) {
        fprintf(stderr, "Reversed buffer overflow\n");
        return 1;
    }
    
    *rev_ptr = '\0';
    
    printf("Words reversed: %s\n", reversed);
    
    return 0;
}