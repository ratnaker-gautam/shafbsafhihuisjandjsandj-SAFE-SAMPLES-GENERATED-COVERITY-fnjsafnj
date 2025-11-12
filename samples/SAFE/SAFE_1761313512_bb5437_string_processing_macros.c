//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    size_t count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    size_t word_count = 0;
    
    printf("Enter text: ");
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
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
            pos++;
            word_len++;
        }
        
        if (word_len == 0) {
            continue;
        }
        
        char current_word[MAX_WORD_LEN];
        for (size_t i = 0; i < word_len; i++) {
            current_word[i] = TO_LOWER(input[word_start + i]);
        }
        current_word[word_len] = '\0';
        
        int found = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                if (words[i].count < SIZE_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
                words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    printf("\nWord frequencies:\n");
    for (size_t i = 0; i < word_count; i++) {
        printf("%s: %zu\n", words[i].word, words[i].count);
    }
    
    return 0;
}