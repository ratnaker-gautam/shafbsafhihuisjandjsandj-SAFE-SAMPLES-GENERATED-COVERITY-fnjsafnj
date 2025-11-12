//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
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
        if (token_len > 0 && token_len < 64) {
            strncpy(words[word_count], token, 63);
            words[word_count][63] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_chars = 0;
    int longest_word_len = 0;
    int shortest_word_len = 64;
    
    for (int i = 0; i < word_count; i++) {
        size_t word_len = strlen(words[i]);
        total_chars += (int)word_len;
        
        if ((int)word_len > longest_word_len) {
            longest_word_len = (int)word_len;
        }
        
        if ((int)word_len < shortest_word_len) {
            shortest_word_len = (int)word_len;
        }
        
        for (size_t j = 0; j < word_len; j++) {
            words[i][j] = (char)toupper((unsigned char)words[i][j]);
        }
        
        printf("Word %d: %s (length: %zu)\n", i + 1, words[i], word_len);
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("\nStatistics:\n");
        printf("Total words: %d\n", word_count);
        printf("Total characters: %d\n", total_chars);
        printf("Average word length: %.2f\n", avg_length);
        printf("Longest word: %d characters\n", longest_word_len);
        printf("Shortest word: %d characters\n", shortest_word_len);
    }
    
    printf("\nProcessing complete.\n");
    return 0;
}