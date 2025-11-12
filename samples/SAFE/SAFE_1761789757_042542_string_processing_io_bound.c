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
    int i, j, k;
    
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
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && j < MAX_WORD_LEN && !isspace((unsigned char)input[i])) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        
        if (j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing words...\n");
    printf("Found %d words:\n", word_count);
    
    for (i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int vowel_count = 0;
        int consonant_count = 0;
        int digit_count = 0;
        
        for (j = 0; j < word_len; j++) {
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
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu, vowels: %d, consonants: %d, digits: %d)\n",
               i + 1, words[i], word_len, vowel_count, consonant_count, digit_count);
    }
    
    printf("\nWord statistics:\n");
    
    int total_chars = 0;
    int total_vowels = 0;
    int total_consonants = 0;
    int total_digits = 0;
    
    for (i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
        
        for (j = 0; j < strlen(words[i]); j++) {
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
            }
        }
    }
    
    printf("Total characters in words: %d\n", total_chars);
    printf("Total vowels: %d\n", total_vowels);
    printf("Total consonants: %d\n", total_consonants);
    printf("Total digits: %d\n", total_digits);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    
    return 0;
}