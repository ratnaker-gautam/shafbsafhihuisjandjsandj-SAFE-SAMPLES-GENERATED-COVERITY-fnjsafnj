//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_string_cpu_bound(const char *input, char *output) {
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    size_t input_len = strlen(input);
    
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                if (word_count < MAX_WORDS) {
                    size_t word_len = i - word_start;
                    if (word_len < MAX_INPUT_LEN) {
                        for (size_t j = 0; j < word_len; j++) {
                            words[word_count][j] = input[word_start + j];
                        }
                        words[word_count][word_len] = '\0';
                        word_count++;
                    }
                }
                in_word = 0;
            }
        }
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strncpy(temp, words[j], MAX_INPUT_LEN - 1);
                temp[MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_INPUT_LEN - 1);
                words[j][MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_INPUT_LEN - 1);
                words[j + 1][MAX_INPUT_LEN - 1] = '\0';
            }
        }
    }
    
    output[0] = '\0';
    for (int i = 0; i < word_count; i++) {
        if (strlen(output) + strlen(words[i]) + 2 < MAX_INPUT_LEN) {
            if (i > 0) {
                strncat(output, " ", MAX_INPUT_LEN - strlen(output) - 1);
            }
            strncat(output, words[i], MAX_INPUT_LEN - strlen(output) - 1);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char output[MAX_INPUT_LEN + 1];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    process_string_cpu_bound(input, output);
    
    printf("Processed output: %s\n", output);
    
    return 0;
}