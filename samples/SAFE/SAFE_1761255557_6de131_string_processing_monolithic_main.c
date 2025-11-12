//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < sizeof(words[0])) {
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        char longest_word[64] = "";
        char shortest_word[64] = "";
        size_t min_len = sizeof(words[0]);
        size_t max_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strcpy(longest_word, words[i]);
            }
            if (len < min_len) {
                min_len = len;
                strcpy(shortest_word, words[i]);
            }
        }
        
        printf("\nLongest word: %s (%zu characters)\n", longest_word, max_len);
        printf("Shortest word: %s (%zu characters)\n", shortest_word, min_len);
    }
    
    return 0;
}