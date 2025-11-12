//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: ", i + 1);
        
        int vowel_count = 0;
        int consonant_count = 0;
        int digit_count = 0;
        int other_count = 0;
        
        for (int j = 0; words[i][j] != '\0'; j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                char lower_c = tolower((unsigned char)c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    vowel_count++;
                } else {
                    consonant_count++;
                }
            } else if (isdigit((unsigned char)c)) {
                digit_count++;
            } else {
                other_count++;
            }
        }
        
        printf("'%s' (vowels: %d, consonants: %d, digits: %d, other: %d)\n",
               words[i], vowel_count, consonant_count, digit_count, other_count);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int total_vowels = 0;
    int total_consonants = 0;
    int total_digits = 0;
    int total_other = 0;
    
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; words[i][j] != '\0'; j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                char lower_c = tolower((unsigned char)c);
                if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                    lower_c == 'o' || lower_c == 'u') {
                    total_vowels++;
                } else {
                    total_consonants++;
                }
            } else if (isdigit((unsigned char)c)) {
                total_digits++;
            } else {
                total_other++;
            }
        }
    }
    
    printf("Total vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
    printf("Total digits: %d\n", total_digits);
    printf("Total other characters: %d\n", total_other);
    
    return 0;
}