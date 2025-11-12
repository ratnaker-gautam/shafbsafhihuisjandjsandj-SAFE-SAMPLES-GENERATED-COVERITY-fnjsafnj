//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int split_into_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!validate_input(input)) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                strncpy(words[*word_count], input + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_palindrome(const char* word) {
    if (word == NULL) return 0;
    size_t len = strlen(word);
    if (len == 0) return 0;
    
    for (size_t i = 0; i < len / 2; i++) {
        if (word[i] != word[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Processing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN];
        strncpy(temp, words[i], MAX_WORD_LEN - 1);
        temp[MAX_WORD_LEN - 1] = '\0';
        
        to_lowercase(temp);
        printf("Word %d: '%s' (length: %zu)", i + 1, words[i], strlen(words[i]));
        
        if (is_palindrome(temp)) {
            printf(" - PALINDROME");
        }
        printf("\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!split_into_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    return 0;
}