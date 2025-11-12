//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int is_valid_word(const char *word, size_t len) {
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isalpha((unsigned char)word[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    size_t len = strlen(word);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        if (isupper((unsigned char)word[i])) {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
    
    if (len > 1) {
        char first = word[0];
        for (size_t i = 0; i < len - 1; i++) {
            word[i] = word[i + 1];
        }
        word[len - 1] = first;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char output[MAX_INPUT_LEN * 2];
    char word[MAX_WORD_LEN];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    output[0] = '\0';
    size_t output_pos = 0;
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < sizeof(word)) {
                    memcpy(word, &input[word_start], word_len);
                    word[word_len] = '\0';
                    
                    if (is_valid_word(word, word_len)) {
                        process_word(word);
                        
                        size_t new_len = strlen(word);
                        if (output_pos + new_len + 1 < sizeof(output)) {
                            memcpy(&output[output_pos], word, new_len);
                            output_pos += new_len;
                            output[output_pos] = ' ';
                            output_pos++;
                            output[output_pos] = '\0';
                        } else {
                            fprintf(stderr, "Output buffer overflow\n");
                            return 1;
                        }
                    }
                }
                in_word = 0;
            }
            
            if (i < input_len && !isalpha((unsigned char)input[i])) {
                if (output_pos + 1 < sizeof(output)) {
                    output[output_pos] = input[i];
                    output_pos++;
                    output[output_pos] = '\0';
                } else {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
            }
        }
    }
    
    printf("Processed text: %s\n", output);
    return 0;
}