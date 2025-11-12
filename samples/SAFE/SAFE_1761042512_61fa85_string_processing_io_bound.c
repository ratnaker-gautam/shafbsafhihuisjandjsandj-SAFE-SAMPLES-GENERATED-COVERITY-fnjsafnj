//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_INPUT_LEN / 2][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < (MAX_INPUT_LEN / 2)) {
        size_t token_len = strlen(token);
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        int letter_count = 0;
        int digit_count = 0;
        
        for (size_t j = 0; j < word_len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu, letters: %d, digits: %d)\n", 
               i + 1, words[i], word_len, letter_count, digit_count);
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_letters = 0;
    int total_digits = 0;
    size_t total_length = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_length += word_len;
        
        for (size_t j = 0; j < word_len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                total_letters++;
            } else if (isdigit((unsigned char)words[i][j])) {
                total_digits++;
            }
        }
    }
    
    printf("Total characters: %zu\n", total_length);
    printf("Total letters: %d\n", total_letters);
    printf("Total digits: %d\n", total_digits);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (double)total_length / word_count);
    }
    
    return 0;
}