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
    int word_lengths[MAX_WORDS];
    
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
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        
        int clean_len = 0;
        for (size_t i = 0; i < token_len; i++) {
            if (isalpha((unsigned char)token[i])) {
                clean_len++;
            }
        }
        word_lengths[word_count] = clean_len;
        
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int total_alpha = 0;
    int longest_word_idx = 0;
    int shortest_word_idx = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        total_alpha += word_lengths[i];
        
        if (word_lengths[i] > word_lengths[longest_word_idx]) {
            longest_word_idx = i;
        }
        if (word_lengths[i] < word_lengths[shortest_word_idx]) {
            shortest_word_idx = i;
        }
        
        printf("Word %d: '%s' (length: %zu, alphabetic: %d)\n", 
               i + 1, words[i], len, word_lengths[i]);
    }
    
    printf("\nStatistics:\n");
    printf("Total characters: %d\n", total_chars);
    printf("Total alphabetic characters: %d\n", total_alpha);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0);
    printf("Longest word: '%s' (%d alphabetic chars)\n", words[longest_word_idx], word_lengths[longest_word_idx]);
    printf("Shortest word: '%s' (%d alphabetic chars)\n", words[shortest_word_idx], word_lengths[shortest_word_idx]);
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}