//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
        return 0;
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
        return 0;
    }
    
    printf("\nWord count: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s\n", words[i]);
    }
    
    printf("\nProcessing words...\n");
    
    int processed_count = 0;
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int has_alpha = 0;
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                has_alpha = 1;
                break;
            }
        }
        
        if (has_alpha) {
            for (size_t j = 0; j < len; j++) {
                if (isupper((unsigned char)words[i][j])) {
                    words[i][j] = tolower((unsigned char)words[i][j]);
                }
            }
            processed_count++;
        }
    }
    
    printf("Processed %d words (converted to lowercase)\n", processed_count);
    printf("\nFinal word list:\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}