//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], size_t *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    if (input_len == 0) return 1;
    
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalnum((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                memcpy(words[*word_count], &input[word_start], word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void sort_words(char words[][MAX_WORD_LEN], size_t word_count) {
    if (words == NULL || word_count == 0) return;
    
    for (size_t i = 0; i < word_count - 1; i++) {
        for (size_t j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], MAX_WORD_LEN);
                strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                strncpy(words[j + 1], temp, MAX_WORD_LEN);
            }
        }
    }
}

int count_word_frequency(const char words[][MAX_WORD_LEN], size_t word_count, 
                        const char *word, size_t *frequency) {
    if (words == NULL || word == NULL || frequency == NULL) return 0;
    
    *frequency = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (strcmp(words[i], word) == 0) {
            (*frequency)++;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    size_t word_count = 0;
    
    printf("Enter text (alphanumeric characters and whitespace only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and whitespace allowed\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    
    size_t processed = 0;
    while (processed < word_count) {
        const char *current_word = words[processed];
        size_t frequency;
        
        if (!count_word_frequency(words, word_count, current_word, &frequency)) {
            fprintf(stderr, "Error counting word frequency\n");
            return 1;
        }
        
        printf("%-20s : %zu\n", current_word, frequency);
        processed += frequency;
    }
    
    printf("\nTotal unique words: %zu\n", word_count);
    
    return 0;
}