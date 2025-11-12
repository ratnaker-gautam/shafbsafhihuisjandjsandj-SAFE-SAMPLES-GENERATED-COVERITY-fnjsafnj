//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

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
        
        char longest_word[MAX_WORD_LEN] = "";
        char shortest_word[MAX_WORD_LEN] = "";
        int longest_len = 0;
        int shortest_len = MAX_WORD_LEN;
        
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            if (len > longest_len) {
                longest_len = len;
                strncpy(longest_word, words[i], MAX_WORD_LEN - 1);
                longest_word[MAX_WORD_LEN - 1] = '\0';
            }
            if (len < shortest_len && len > 0) {
                shortest_len = len;
                strncpy(shortest_word, words[i], MAX_WORD_LEN - 1);
                shortest_word[MAX_WORD_LEN - 1] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%d characters)\n", longest_word, longest_len);
        printf("Shortest word: %s (%d characters)\n", shortest_word, shortest_len);
        
        int uppercase_count = 0;
        int lowercase_count = 0;
        int digit_count = 0;
        int punctuation_count = 0;
        
        for (int i = 0; i < char_count; i++) {
            unsigned char c = input[i];
            if (isupper(c)) uppercase_count++;
            else if (islower(c)) lowercase_count++;
            else if (isdigit(c)) digit_count++;
            else if (ispunct(c)) punctuation_count++;
        }
        
        printf("\nCharacter Statistics:\n");
        printf("Uppercase letters: %d\n", uppercase_count);
        printf("Lowercase letters: %d\n", lowercase_count);
        printf("Digits: %d\n", digit_count);
        printf("Punctuation: %d\n", punctuation_count);
    }
    
    return 0;
}