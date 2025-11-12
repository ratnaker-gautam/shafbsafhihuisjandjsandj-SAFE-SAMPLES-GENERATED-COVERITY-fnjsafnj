//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_chars = 0;
    int uppercase_count = 0;
    int lowercase_count = 0;
    int digit_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: %s\n", i + 1, words[i]);
        
        for (int j = 0; words[i][j] != '\0'; j++) {
            char c = words[i][j];
            total_chars++;
            
            if (isupper(c)) {
                uppercase_count++;
            } else if (islower(c)) {
                lowercase_count++;
            } else if (isdigit(c)) {
                digit_count++;
            }
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Uppercase letters: %d\n", uppercase_count);
    printf("Lowercase letters: %d\n", lowercase_count);
    printf("Digits: %d\n", digit_count);
    
    if (total_chars > 0) {
        printf("Uppercase percentage: %.2f%%\n", (uppercase_count * 100.0) / total_chars);
        printf("Lowercase percentage: %.2f%%\n", (lowercase_count * 100.0) / total_chars);
        printf("Digit percentage: %.2f%%\n", (digit_count * 100.0) / total_chars);
    }
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}