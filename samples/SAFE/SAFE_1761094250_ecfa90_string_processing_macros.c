//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    size_t count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    size_t word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input_len >= sizeof(input) - 1 && input[input_len - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t pos = 0;
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        size_t word_start = pos;
        size_t word_len = 0;
        char current_word[MAX_WORD_LEN];
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
            current_word[word_len] = input[pos];
            word_len++;
            pos++;
        }
        
        if (word_len == 0) {
            continue;
        }
        
        current_word[word_len] = '\0';
        
        for (size_t i = 0; i < word_len; i++) {
            if (IS_TERMINATOR(current_word[i])) {
                current_word[i] = '\0';
                word_len = i;
                break;
            }
            current_word[i] = TO_LOWER(current_word[i]);
        }
        
        if (word_len == 0) {
            continue;
        }
        
        int found = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (size_t i = 0; i < word_count; i++) {
        char display_word[MAX_WORD_LEN];
        strncpy(display_word, words[i].word, MAX_WORD_LEN - 1);
        display_word[MAX_WORD_LEN - 1] = '\0';
        
        if (strlen(display_word) > 0) {
            display_word[0] = TO_UPPER(display_word[0]);
        }
        
        printf("%-20s: %zu\n", display_word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
    }
    
    return 0;
}