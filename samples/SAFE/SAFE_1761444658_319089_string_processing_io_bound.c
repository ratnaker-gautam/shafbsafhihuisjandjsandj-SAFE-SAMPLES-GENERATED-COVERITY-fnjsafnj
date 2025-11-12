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
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    size_t len = strlen(word);
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
    char output[MAX_INPUT_LEN * 2];
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
    
    output[0] = '\0';
    size_t output_pos = 0;
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace(input[i]) && word_pos < MAX_WORD_LEN - 1) {
            word[word_pos++] = input[i];
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                process_word(word);
                
                size_t word_len = strlen(word);
                if (output_pos + word_len + 1 < sizeof(output)) {
                    if (output_pos > 0) {
                        output[output_pos++] = ' ';
                    }
                    strcpy(output + output_pos, word);
                    output_pos += word_len;
                }
                word_pos = 0;
            }
        }
    }
    
    output[output_pos] = '\0';
    printf("Processed text: %s\n", output);
    
    return 0;
}