//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strncpy(words[word_count], token, token_len + 1);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int char_count = 0;
    int digit_count = 0;
    int alpha_count = 0;
    int upper_count = 0;
    int lower_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char_count += strlen(words[i]);
        
        for (size_t j = 0; j < strlen(words[i]); j++) {
            unsigned char c = words[i][j];
            if (isdigit(c)) {
                digit_count++;
            }
            if (isalpha(c)) {
                alpha_count++;
                if (isupper(c)) {
                    upper_count++;
                } else {
                    lower_count++;
                }
            }
        }
    }
    
    printf("Total characters in words: %d\n", char_count);
    printf("Digits: %d\n", digit_count);
    printf("Letters: %d\n", alpha_count);
    printf("Uppercase: %d\n", upper_count);
    printf("Lowercase: %d\n", lower_count);
    
    printf("\nWords sorted by length:\n");
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strlen(words[i]) > strlen(words[j])) {
                char *temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        printf("%s (%zu)\n", words[i], strlen(words[i]));
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}