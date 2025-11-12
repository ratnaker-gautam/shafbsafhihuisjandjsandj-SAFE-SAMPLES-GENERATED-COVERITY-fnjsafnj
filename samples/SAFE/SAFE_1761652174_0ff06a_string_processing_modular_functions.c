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
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int split_into_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!validate_input(input)) return 0;
    
    *word_count = 0;
    int pos = 0;
    int len = strlen(input);
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && isspace(input[pos])) pos++;
        if (pos >= len) break;
        
        int word_pos = 0;
        while (pos < len && !isspace(input[pos]) && word_pos < MAX_WORD_LEN - 1) {
            words[*word_count][word_pos++] = input[pos++];
        }
        words[*word_count][word_pos] = '\0';
        (*word_count)++;
    }
    
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int is_palindrome(const char* word) {
    if (word == NULL) return 0;
    
    int len = strlen(word);
    if (len == 0) return 0;
    
    for (int i = 0; i < len / 2; i++) {
        if (word[i] != word[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    int palindrome_count = 0;
    
    printf("Processing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        char original[MAX_WORD_LEN];
        strncpy(original, words[i], MAX_WORD_LEN - 1);
        original[MAX_WORD_LEN - 1] = '\0';
        
        to_lowercase(words[i]);
        
        printf("Word %d: '%s' -> '%s'", i + 1, original, words[i]);
        
        if (is_palindrome(words[i])) {
            printf(" [PALINDROME]");
            palindrome_count++;
        }
        printf("\n");
    }
    
    printf("\nFound %d palindrome(s) in the text.\n", palindrome_count);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text to process (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input: too long or empty.\n");
        return 1;
    }
    
    if (!split_into_words(input, words, &word_count)) {
        printf("Error splitting input into words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    process_words(words, word_count);
    
    return 0;
}