//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500
#define MAX_WORD_LEN 50

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

void extract_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    *word_count = 0;
    size_t input_len = strlen(input);
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
                if (word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count], input + word_start, word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
                in_word = 0;
            }
        }
    }
}

void sort_words(char words[][MAX_WORD_LEN], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], MAX_WORD_LEN);
                strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                strncpy(words[j + 1], temp, MAX_WORD_LEN);
            }
        }
    }
}

void process_string(const char *input) {
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    extract_words(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return;
    }
    
    sort_words(words, word_count);
    
    printf("Sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord frequency:\n");
    int freq = 1;
    for (int i = 1; i <= word_count; i++) {
        if (i < word_count && strcmp(words[i], words[i - 1]) == 0) {
            freq++;
        } else {
            printf("%s: %d\n", words[i - 1], freq);
            freq = 1;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input. Only alphanumeric characters and whitespace allowed.\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input.\n");
        return 1;
    }
    
    process_string(input);
    
    return 0;
}