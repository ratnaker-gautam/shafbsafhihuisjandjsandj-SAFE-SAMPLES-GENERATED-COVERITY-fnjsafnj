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
        if (token_len > 0) {
            if (token_len <= MAX_WORD_LEN) {
                strncpy(words[word_count], token, MAX_WORD_LEN);
                words[word_count][MAX_WORD_LEN] = '\0';
                word_count++;
            } else {
                fprintf(stderr, "Word too long: %s\n", token);
                return 1;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count == MAX_WORDS && token != NULL) {
        fprintf(stderr, "Too many words\n");
        return 1;
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
            printf("%s: %zu\n", words[i], strlen(words[i]));
        }
        
        char longest_word[MAX_WORD_LEN + 1] = "";
        size_t max_len = 0;
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words[i]);
            if (len > max_len) {
                max_len = len;
                strncpy(longest_word, words[i], MAX_WORD_LEN);
                longest_word[MAX_WORD_LEN] = '\0';
            }
        }
        
        printf("\nLongest word: %s (%zu characters)\n", longest_word, max_len);
        
        int uppercase_count = 0;
        int lowercase_count = 0;
        int digit_count = 0;
        
        for (int i = 0; i < word_count; i++) {
            for (size_t j = 0; j < strlen(words[i]); j++) {
                if (isupper((unsigned char)words[i][j])) {
                    uppercase_count++;
                } else if (islower((unsigned char)words[i][j])) {
                    lowercase_count++;
                } else if (isdigit((unsigned char)words[i][j])) {
                    digit_count++;
                }
            }
        }
        
        printf("\nCharacter Statistics:\n");
        printf("Uppercase letters: %d\n", uppercase_count);
        printf("Lowercase letters: %d\n", lowercase_count);
        printf("Digits: %d\n", digit_count);
    }
    
    return 0;
}