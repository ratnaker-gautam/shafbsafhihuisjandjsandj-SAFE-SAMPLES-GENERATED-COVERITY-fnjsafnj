//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    size_t len = strlen(word);
    int has_alpha = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            has_alpha = 1;
            break;
        }
    }
    
    if (has_alpha) {
        for (size_t i = 0; i < len; i++) {
            if (isalpha((unsigned char)word[i])) {
                word[i] = toupper((unsigned char)word[i]);
            }
        }
    } else {
        for (size_t i = 0; i < len; i++) {
            if (isdigit((unsigned char)word[i])) {
                word[i] = 'X';
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN] = {0};
    char word[MAX_WORD_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len-1] == '\n') {
        input[input_len-1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    while (input_pos < input_len && output_pos < MAX_INPUT_LEN - 1) {
        while (input_pos < input_len && isspace((unsigned char)input[input_pos])) {
            if (output_pos < MAX_INPUT_LEN - 1) {
                output[output_pos++] = input[input_pos++];
            } else {
                break;
            }
        }
        
        if (input_pos >= input_len) break;
        
        size_t word_len = 0;
        while (input_pos < input_len && !isspace((unsigned char)input[input_pos]) && word_len < MAX_WORD_LEN - 1) {
            word[word_len++] = input[input_pos++];
        }
        word[word_len] = '\0';
        
        process_word(word);
        
        size_t processed_len = strlen(word);
        if (output_pos + processed_len < MAX_INPUT_LEN - 1) {
            strcpy(&output[output_pos], word);
            output_pos += processed_len;
        } else {
            break;
        }
    }
    
    output[output_pos] = '\0';
    printf("Processed text: %s\n", output);
    
    return 0;
}