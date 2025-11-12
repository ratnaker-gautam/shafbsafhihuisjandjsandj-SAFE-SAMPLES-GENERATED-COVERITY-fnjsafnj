//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_WORD_LENGTH 256

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LENGTH) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    return 1;
}

void process_word(char *word, size_t len) {
    if (word == NULL || len == 0 || len > MAX_WORD_LENGTH) {
        return;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (isalpha(word[i])) {
            if (islower(word[i])) {
                word[i] = (char)toupper(word[i]);
            } else {
                word[i] = (char)tolower(word[i]);
            }
        }
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = word[i];
        word[i] = word[len - 1 - i];
        word[len - 1 - i] = temp;
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH + 1];
    char output[MAX_INPUT_LENGTH + 1];
    char word[MAX_WORD_LENGTH + 1];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    output[0] = '\0';
    size_t output_pos = 0;
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace(input[i]) && word_pos < MAX_WORD_LENGTH) {
            word[word_pos++] = input[i];
        } else {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                process_word(word, word_pos);
                
                size_t word_len = strlen(word);
                if (output_pos + word_len + 1 < sizeof(output)) {
                    if (output_pos > 0) {
                        output[output_pos++] = ' ';
                    }
                    memcpy(output + output_pos, word, word_len);
                    output_pos += word_len;
                } else {
                    fprintf(stderr, "Output buffer overflow\n");
                    return EXIT_FAILURE;
                }
                word_pos = 0;
            }
            
            if (i < input_len && isspace(input[i]) && output_pos < sizeof(output) - 1) {
                output[output_pos++] = input[i];
            }
        }
    }
    
    output[output_pos] = '\0';
    
    printf("Processed text: %s\n", output);
    
    return EXIT_SUCCESS;
}