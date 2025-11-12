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
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_len) {
            longest_len = len;
        }
        if (len < shortest_len) {
            shortest_len = len;
        }
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords in reverse order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("\nWords sorted by length:\n");
    for (int len = shortest_len; len <= longest_len; len++) {
        for (int i = 0; i < word_count; i++) {
            if (strlen(words[i]) == len) {
                printf("%s ", words[i]);
            }
        }
    }
    printf("\n");
    
    return 0;
}