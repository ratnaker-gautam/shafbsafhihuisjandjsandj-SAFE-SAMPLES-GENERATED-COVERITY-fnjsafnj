//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord Lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %zu characters\n", words[i], strlen(words[i]));
        }
        
        char longest_word[MAX_WORD_LEN] = "";
        char shortest_word[MAX_WORD_LEN] = "";
        size_t max_len = 0;
        size_t min_len = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strncpy(longest_word, words[i], MAX_WORD_LEN - 1);
                longest_word[MAX_WORD_LEN - 1] = '\0';
            }
            if (len < min_len) {
                min_len = len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN - 1);
                shortest_word[MAX_WORD_LEN - 1] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%zu characters)\n", longest_word, max_len);
        printf("Shortest word: %s (%zu characters)\n", shortest_word, min_len);
    }
    
    char processed[MAX_INPUT_LEN];
    size_t proc_idx = 0;
    
    for (size_t i = 0; i < input_len && proc_idx < MAX_INPUT_LEN - 1; i++) {
        if (isalpha(input[i])) {
            processed[proc_idx++] = toupper(input[i]);
        } else if (isspace(input[i])) {
            processed[proc_idx++] = ' ';
        }
    }
    processed[proc_idx] = '\0';
    
    printf("\nProcessed text (letters only, uppercase):\n%s\n", processed);
    
    return 0;
}