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
        printf("No input provided\n");
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
        printf("No words found in input\n");
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
            printf("Word %d: '%s' -> (no alphabetic characters)\n", 
                   i + 1, words[i]);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    int processed_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == 0) continue;
        
        int duplicate = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) {
            int count = 1;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    count++;
                }
            }
            printf("'%s' appears %d time(s)\n", words[i], count);
            processed_count++;
        }
    }
    
    printf("Total unique words: %d\n", processed_count);
    
    return 0;
}