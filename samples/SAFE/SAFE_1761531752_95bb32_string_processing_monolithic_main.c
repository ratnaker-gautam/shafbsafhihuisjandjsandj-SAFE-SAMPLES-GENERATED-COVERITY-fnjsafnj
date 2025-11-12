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
    
    if (len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = len;
    
    for (int i = 0; i < len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    line_count++;
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len < sizeof(words[0])) {
            strncpy(words[word_count], token, sizeof(words[0]) - 1);
            words[word_count][sizeof(words[0]) - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
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
            int word_len = 0;
            for (int j = 0; words[i][j] != '\0'; j++) {
                if (isalnum((unsigned char)words[i][j])) {
                    word_len++;
                }
            }
            printf("%s: %d alphanumeric characters\n", words[i], word_len);
        }
        
        char longest_word[64] = "";
        int max_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            int current_len = strlen(words[i]);
            if (current_len > max_len) {
                max_len = current_len;
                strncpy(longest_word, words[i], sizeof(longest_word) - 1);
                longest_word[sizeof(longest_word) - 1] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%d characters)\n", longest_word, max_len);
    }
    
    return 0;
}