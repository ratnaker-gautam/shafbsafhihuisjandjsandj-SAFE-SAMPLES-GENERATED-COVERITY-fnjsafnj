//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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
    if (input_len >= sizeof(input)) {
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
        
        while (pos < input_len && word_len < (MAX_WORD_LEN - 1)) {
            char c = input[pos];
            if (IS_WHITESPACE(c)) {
                break;
            }
            current_word[word_len] = TO_LOWER(c);
            word_len++;
            pos++;
        }
        
        if (word_len == 0) {
            continue;
        }
        
        current_word[word_len] = '\0';
        
        int valid_word = 0;
        for (size_t i = 0; i < word_len; i++) {
            if (IS_ALPHA(current_word[i])) {
                valid_word = 1;
                break;
            }
        }
        
        if (!valid_word) {
            continue;
        }
        
        size_t existing_index = MAX_WORDS;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                existing_index = i;
                break;
            }
        }
        
        if (existing_index < MAX_WORDS) {
            if (words[existing_index].count < SIZE_MAX) {
                words[existing_index].count++;
            }
        } else {
            if (word_count < MAX_WORDS) {
                strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
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