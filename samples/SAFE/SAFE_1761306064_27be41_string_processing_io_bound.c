//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_word(const char *word, size_t len) {
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) return 0;
    }
    return 1;
}

void process_word(char *word, size_t len) {
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        if (i == 0) {
            word[i] = toupper((unsigned char)word[i]);
        } else {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char output[MAX_INPUT_LEN * 2 + 1];
    char word[MAX_WORD_LEN + 1];
    
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    output[0] = '\0';
    size_t output_pos = 0;
    size_t word_pos = 0;
    size_t i = 0;
    
    while (i < input_len) {
        if (isspace((unsigned char)input[i])) {
            if (word_pos > 0) {
                word[word_pos] = '\0';
                if (is_valid_word(word, word_pos)) {
                    process_word(word, word_pos);
                }
                
                size_t word_len = strlen(word);
                if (output_pos + word_len + 1 >= sizeof(output)) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                
                strcpy(output + output_pos, word);
                output_pos += word_len;
                word_pos = 0;
            }
            
            if (output_pos + 1 >= sizeof(output)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            output[output_pos++] = input[i];
            output[output_pos] = '\0';
        } else {
            if (word_pos >= MAX_WORD_LEN) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            word[word_pos++] = input[i];
        }
        i++;
    }
    
    if (word_pos > 0) {
        word[word_pos] = '\0';
        if (is_valid_word(word, word_pos)) {
            process_word(word, word_pos);
        }
        
        size_t word_len = strlen(word);
        if (output_pos + word_len >= sizeof(output)) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        strcpy(output + output_pos, word);
    }
    
    printf("Processed text: %s\n", output);
    return 0;
}