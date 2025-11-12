//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    if (len < 2) return;
    
    for (size_t i = 0; i < len; i++) {
        if (i % 2 == 0) {
            word[i] = toupper(word[i]);
        } else {
            word[i] = tolower(word[i]);
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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    size_t input_pos = 0;
    size_t output_pos = 0;
    
    while (input_pos < input_len && output_pos < sizeof(output) - 1) {
        size_t word_len = 0;
        
        while (input_pos < input_len && isspace(input[input_pos])) {
            if (output_pos < sizeof(output) - 1) {
                output[output_pos++] = input[input_pos++];
            } else {
                break;
            }
        }
        
        while (input_pos < input_len && !isspace(input[input_pos]) && word_len < MAX_WORD_LEN - 1) {
            word[word_len++] = input[input_pos++];
        }
        word[word_len] = '\0';
        
        if (word_len > 0) {
            process_word(word);
            size_t processed_len = strlen(word);
            if (output_pos + processed_len < sizeof(output) - 1) {
                strcpy(&output[output_pos], word);
                output_pos += processed_len;
            } else {
                break;
            }
        }
    }
    
    output[output_pos] = '\0';
    printf("Processed text: %s\n", output);
    
    return 0;
}