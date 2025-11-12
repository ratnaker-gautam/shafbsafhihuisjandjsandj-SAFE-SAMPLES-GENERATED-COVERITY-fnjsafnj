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
        printf("Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            if (token_len > MAX_WORD_LEN) {
                printf("Word '%.*s...' too long, skipping\n", 10, token);
            } else {
                strncpy(words[word_count], token, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: '%s' -> ", i + 1, words[i]);
        
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
        
        printf("'%s'\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    printf("Total characters: %d\n", total_chars);
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (double)total_chars / word_count);
    }
    
    return 0;
}