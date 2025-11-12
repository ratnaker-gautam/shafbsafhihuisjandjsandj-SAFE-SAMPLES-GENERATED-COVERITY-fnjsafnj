//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        int digit_count = 0;
        
        for (size_t j = 0; j < len; j++) {
            char c = words[i][j];
            if (isdigit(c)) {
                digit_count++;
            } else if (isalpha(c)) {
                char lower_c = tolower(c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            }
        }
        
        printf("Word %d: \"%s\" (Length: %zu, Vowels: %d, Consonants: %d, Digits: %d)\n",
               i + 1, words[i], len, vowel_count, consonant_count, digit_count);
    }
    
    printf("\nWord statistics:\n");
    int total_vowels = 0;
    int total_consonants = 0;
    int total_digits = 0;
    size_t total_length = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_length += len;
        
        for (size_t j = 0; j < len; j++) {
            char c = words[i][j];
            if (isdigit(c)) {
                total_digits++;
            } else if (isalpha(c)) {
                char lower_c = tolower(c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    total_vowels++;
                } else {
                    total_consonants++;
                }
            }
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total characters: %zu\n", total_length);
    printf("Total vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
    printf("Total digits: %d\n", total_digits);
    
    return 0;
}