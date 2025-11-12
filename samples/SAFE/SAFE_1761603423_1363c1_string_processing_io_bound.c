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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int letter_count = 0;
        int digit_count = 0;
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
        }
        
        printf("Word %d: \"%s\" (letters: %d, digits: %d, length: %zu)\n", 
               i + 1, words[i], letter_count, digit_count, len);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_letters = 0;
    int total_digits = 0;
    size_t total_chars = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                total_letters++;
            } else if (isdigit((unsigned char)words[i][j])) {
                total_digits++;
            }
        }
    }
    
    printf("Total characters: %zu\n", total_chars);
    printf("Total letters: %d\n", total_letters);
    printf("Total digits: %d\n", total_digits);
    
    if (total_chars > 0) {
        printf("Average word length: %.2f\n", (double)total_chars / word_count);
    }
    
    return 0;
}