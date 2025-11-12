//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    int char_count = 0;
    int line_count = 0;
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (isprint((unsigned char)input[i])) {
            char_count++;
        }
        if (input[i] == '\n') {
            line_count++;
        }
    }
    if (input_len > 0 && input[input_len - 1] != '\n') {
        line_count++;
    }
    
    printf("\nInput analysis:\n");
    printf("Original text: %s\n", input);
    printf("Character count: %d\n", char_count);
    printf("Line count: %d\n", line_count);
    printf("Word count: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWords found:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord lengths:\n");
        for (int i = 0; i < word_count; i++) {
            printf("'%s': %zu characters\n", words[i], strlen(words[i]));
        }
        
        char longest_word[MAX_WORD_LEN + 1] = "";
        char shortest_word[MAX_WORD_LEN + 1] = "";
        size_t max_len = 0;
        size_t min_len = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strncpy(longest_word, words[i], MAX_WORD_LEN);
                longest_word[MAX_WORD_LEN] = '\0';
            }
            if (len < min_len && len > 0) {
                min_len = len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN);
                shortest_word[MAX_WORD_LEN] = '\0';
            }
        }
        
        if (max_len > 0) {
            printf("\nLongest word: '%s' (%zu characters)\n", longest_word, max_len);
        }
        if (min_len < MAX_WORD_LEN && min_len > 0) {
            printf("Shortest word: '%s' (%zu characters)\n", shortest_word, min_len);
        }
    }
    
    return 0;
}