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
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    printf("Word count: %d\n", word_count);
    
    int total_length = 0;
    int longest_word_index = 0;
    int shortest_word_index = 0;
    
    for (int i = 0; i < word_count; i++) {
        int word_len = strlen(words[i]);
        total_length += word_len;
        
        if (word_len > strlen(words[longest_word_index])) {
            longest_word_index = i;
        }
        if (word_len < strlen(words[shortest_word_index])) {
            shortest_word_index = i;
        }
        
        for (int j = 0; j < word_len; j++) {
            words[i][j] = tolower((unsigned char)words[i][j]);
        }
    }
    
    printf("Average word length: %.2f\n", (double)total_length / word_count);
    printf("Longest word: %s (%zu characters)\n", words[longest_word_index], strlen(words[longest_word_index]));
    printf("Shortest word: %s (%zu characters)\n", words[shortest_word_index], strlen(words[shortest_word_index]));
    
    printf("\nWords in lowercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        int frequency = 1;
        if (words[i][0] != '\0') {
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    frequency++;
                    words[j][0] = '\0';
                }
            }
            printf("'%s': %d\n", words[i], frequency);
        }
    }
    
    return 0;
}