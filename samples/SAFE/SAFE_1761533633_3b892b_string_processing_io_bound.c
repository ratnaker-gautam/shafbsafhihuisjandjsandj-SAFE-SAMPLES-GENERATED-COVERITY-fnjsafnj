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
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        int valid_word = 1;
        for (size_t i = 0; i < token_len; i++) {
            if (!isalpha((unsigned char)token[i])) {
                valid_word = 0;
                break;
            }
        }
        
        if (valid_word) {
            strcpy(words[word_count], token);
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
        char processed[MAX_WORD_LEN];
        
        for (size_t j = 0; j < len; j++) {
            if (j == 0) {
                processed[j] = toupper((unsigned char)words[i][j]);
            } else {
                processed[j] = tolower((unsigned char)words[i][j]);
            }
        }
        processed[len] = '\0';
        
        printf("Word %d: %s -> %s\n", i + 1, words[i], processed);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += strlen(words[i]);
    }
    printf("Total characters: %d\n", total_chars);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (double)total_chars / word_count);
    }
    
    return 0;
}