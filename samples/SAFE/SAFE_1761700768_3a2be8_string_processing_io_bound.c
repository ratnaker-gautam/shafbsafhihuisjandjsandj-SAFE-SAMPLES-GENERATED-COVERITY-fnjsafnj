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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty input\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
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
        printf("No words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t j = 0;
        size_t k = 0;
        
        while (words[i][j] != '\0' && k < MAX_WORD_LEN) {
            if (isalpha((unsigned char)words[i][j])) {
                processed[k] = tolower((unsigned char)words[i][j]);
                k++;
            }
            j++;
        }
        processed[k] = '\0';
        
        if (k > 0) {
            printf("Word %d: '%s' -> '%s' (length: %zu)\n", 
                   i + 1, words[i], processed, k);
        } else {
            printf("Word %d: '%s' -> (no letters)\n", i + 1, words[i]);
        }
    }
    
    printf("\nWord count: %d\n", word_count);
    
    int letter_count = 0;
    for (int i = 0; i < word_count; i++) {
        for (size_t j = 0; words[i][j] != '\0'; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            }
        }
    }
    
    printf("Total letters: %d\n", letter_count);
    
    return 0;
}