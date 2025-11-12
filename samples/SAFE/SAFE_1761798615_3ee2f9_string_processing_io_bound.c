//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    size_t input_len = strlen(input);
    size_t output_idx = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isalpha((unsigned char)input[i])) {
            if (!in_word) {
                if (output_idx > 0 && output_idx < MAX_INPUT_LEN - 1) {
                    output[output_idx++] = ' ';
                }
                in_word = 1;
            }
            if (output_idx < MAX_INPUT_LEN - 1) {
                output[output_idx++] = input[i];
            }
        } else {
            in_word = 0;
        }
    }
    
    if (output_idx > 0 && output[output_idx - 1] == ' ') {
        output_idx--;
    }
    output[output_idx] = '\0';
}

int main() {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN];
    
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
    
    process_string(input, output);
    
    printf("Processed text: %s\n", output);
    
    return 0;
}