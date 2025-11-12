//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int word_lengths[MAX_WORDS];
    int total_chars = 0;
    int longest_word_idx = -1;
    int shortest_word_idx = -1;
    
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
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        int valid_word = 1;
        for (size_t i = 0; i < token_len; i++) {
            if (!isalpha((unsigned char)token[i])) {
                valid_word = 0;
                break;
            }
        }
        
        if (valid_word && token_len > 0) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_lengths[word_count] = (int)token_len;
            total_chars += (int)token_len;
            
            if (longest_word_idx == -1 || token_len > (size_t)word_lengths[longest_word_idx]) {
                longest_word_idx = word_count;
            }
            
            if (shortest_word_idx == -1 || token_len < (size_t)word_lengths[shortest_word_idx]) {
                shortest_word_idx = word_count;
            }
            
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nText analysis results:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters in words: %d\n", total_chars);
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("Average word length: %.2f\n", avg_length);
    }
    
    if (longest_word_idx != -1) {
        printf("Longest word: %s (%d characters)\n", 
               words[longest_word_idx], word_lengths[longest_word_idx]);
    }
    
    if (shortest_word_idx != -1) {
        printf("Shortest word: %s (%d characters)\n", 
               words[shortest_word_idx], word_lengths[shortest_word_idx]);
    }
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s", words[i]);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}