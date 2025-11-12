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
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
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
        }
        
        printf("Processed: %s\n", words[i]);
    }
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += (int)len;
        
        if ((int)len < shortest_len) {
            shortest_len = (int)len;
        }
        if ((int)len > longest_len) {
            longest_len = (int)len;
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0f);
    
    return 0;
}