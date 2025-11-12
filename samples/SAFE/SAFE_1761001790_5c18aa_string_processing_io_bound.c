//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_string(const char *input, char *output) {
    size_t len = strlen(input);
    size_t out_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                if (out_idx > 0 && out_idx < MAX_INPUT_LEN - 1) {
                    output[out_idx++] = ' ';
                }
                in_word = 1;
            }
            if (out_idx < MAX_INPUT_LEN - 1) {
                output[out_idx++] = input[i];
            }
        } else {
            in_word = 0;
        }
    }
    output[out_idx] = '\0';
}

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)str[i])) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    return count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char processed[MAX_INPUT_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string(input, processed);
    
    int word_count = count_words(processed);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %d\n", word_count);
    
    return 0;
}