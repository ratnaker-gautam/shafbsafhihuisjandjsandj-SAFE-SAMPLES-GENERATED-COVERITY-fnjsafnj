//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    for (size_t i = 0; i < strlen(input); i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int split_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    if (input_len == 0) return 1;
    
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        if (isalnum((unsigned char)c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                strncpy(words[*word_count], &input[word_start], word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int compare_words(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
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
        fprintf(stderr, "Invalid input: contains invalid characters\n");
        return 1;
    }
    
    if (!split_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words: too many words or word too long\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, MAX_WORD_LEN, compare_words);
    
    printf("Sorted words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}