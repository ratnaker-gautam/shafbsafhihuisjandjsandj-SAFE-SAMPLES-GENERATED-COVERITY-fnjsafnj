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
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
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
        char processed[MAX_WORD_LEN];
        size_t len = strlen(words[i]);
        size_t j = 0;
        
        for (size_t k = 0; k < len && j < MAX_WORD_LEN - 1; k++) {
            if (isalnum((unsigned char)words[i][k])) {
                processed[j++] = tolower((unsigned char)words[i][k]);
            }
        }
        processed[j] = '\0';
        
        if (j > 0) {
            printf("Word %d: '%s' -> '%s' (%zu chars)\n", 
                   i + 1, words[i], processed, j);
        } else {
            printf("Word %d: '%s' -> [empty]\n", i + 1, words[i]);
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int alnum_only_count = 0;
    int max_len = 0;
    int min_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int is_alnum_only = 1;
        
        for (size_t j = 0; j < len; j++) {
            if (!isalnum((unsigned char)words[i][j])) {
                is_alnum_only = 0;
                break;
            }
        }
        
        if (is_alnum_only) {
            alnum_only_count++;
        }
        
        if ((int)len > max_len) max_len = (int)len;
        if ((int)len < min_len) min_len = (int)len;
    }
    
    printf("Alphanumeric-only words: %d\n", alnum_only_count);
    printf("Longest word length: %d\n", max_len);
    printf("Shortest word length: %d\n", min_len);
    
    return 0;
}