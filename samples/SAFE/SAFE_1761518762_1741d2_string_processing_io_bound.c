//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LENGTH) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[i] = toupper((unsigned char)word[i]);
        }
    }
}

int split_into_words(const char *input, char words[][64]) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
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
                if (word_len < 64 && word_count < MAX_WORD_COUNT) {
                    strncpy(words[word_count], &input[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    char words[MAX_WORD_COUNT][64];
    
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
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words);
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        char original[64];
        strncpy(original, words[i], sizeof(original) - 1);
        original[sizeof(original) - 1] = '\0';
        
        process_word(words[i]);
        printf("Word %d: '%s' -> '%s'\n", i + 1, original, words[i]);
    }
    
    printf("Processing complete.\n");
    return 0;
}