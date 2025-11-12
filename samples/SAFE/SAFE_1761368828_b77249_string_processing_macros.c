//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
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
    
    size_t i = 0;
    while (i < input_len) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        char current_word[MAX_WORD_LEN];
        size_t word_len = 0;
        int has_terminator = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            if (IS_TERMINATOR(input[i])) {
                has_terminator = 1;
            }
            current_word[word_len++] = TO_LOWER(input[i]);
            i++;
        }
        
        if (word_len == 0) {
            continue;
        }
        
        current_word[word_len] = '\0';
        
        if (has_terminator) {
            size_t last_char = word_len - 1;
            while (last_char > 0 && IS_TERMINATOR(current_word[last_char])) {
                current_word[last_char] = '\0';
                last_char--;
                word_len--;
            }
        }
        
        if (word_len == 0) {
            continue;
        }
        
        int found = 0;
        for (size_t j = 0; j < word_count; j++) {
            if (strcmp(words[j].word, current_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many unique words\n");
                return 1;
            }
            strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
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
    
    return 0;
}