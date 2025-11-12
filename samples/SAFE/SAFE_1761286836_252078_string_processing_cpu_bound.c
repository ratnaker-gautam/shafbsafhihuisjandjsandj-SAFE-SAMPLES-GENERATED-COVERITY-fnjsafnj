//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_string(const char *input, char *output) {
    size_t len = strlen(input);
    size_t out_idx = 0;
    
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if (isalpha(c)) {
            if (islower(c)) {
                output[out_idx++] = (char)toupper(c);
            } else {
                output[out_idx++] = (char)tolower(c);
            }
        } else if (isdigit(c)) {
            int digit = c - '0';
            digit = (digit * 7 + 3) % 10;
            output[out_idx++] = (char)('0' + digit);
        } else if (isspace(c)) {
            output[out_idx++] = ' ';
        } else {
            output[out_idx++] = '_';
        }
    }
    output[out_idx] = '\0';
}

int count_words(const char *str) {
    int count = 0;
    int in_word = 0;
    size_t len = strlen(str);
    
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
    }
    return count;
}

void find_longest_word(const char *str, char *longest) {
    size_t len = strlen(str);
    size_t max_len = 0;
    size_t current_start = 0;
    size_t current_len = 0;
    longest[0] = '\0';
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && !isspace((unsigned char)str[i])) {
            if (current_len == 0) current_start = i;
            current_len++;
        } else {
            if (current_len > max_len) {
                max_len = current_len;
                if (max_len < MAX_INPUT_LEN) {
                    strncpy(longest, &str[current_start], max_len);
                    longest[max_len] = '\0';
                }
            }
            current_len = 0;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    char longest_word[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
    
    process_string(input, processed);
    
    int word_count = count_words(input);
    find_longest_word(input, longest_word);
    
    printf("Original string: %s\n", input);
    printf("Processed string: %s\n", processed);
    printf("Word count: %d\n", word_count);
    printf("Longest word: %s\n", longest_word);
    
    return 0;
}