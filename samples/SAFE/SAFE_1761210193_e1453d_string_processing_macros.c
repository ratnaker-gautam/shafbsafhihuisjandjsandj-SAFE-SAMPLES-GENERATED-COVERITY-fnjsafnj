//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char text[MAX_WORD_LEN];
    size_t length;
    int count;
} WordInfo;

int main(void) {
    char input[MAX_INPUT_LEN];
    WordInfo words[MAX_WORDS];
    size_t word_count = 0;
    size_t input_len;
    size_t i, j;
    int c;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input_len = strlen(input);
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        for (j = 0; j < word_len; j++) {
            c = input[word_start + j];
            current_word[j] = TO_LOWER(c);
        }
        current_word[word_len] = '\0';
        
        size_t clean_len = 0;
        for (j = 0; j < word_len; j++) {
            if (IS_ALPHA(current_word[j])) {
                current_word[clean_len++] = current_word[j];
            }
        }
        current_word[clean_len] = '\0';
        
        if (clean_len == 0) {
            continue;
        }
        
        int found = 0;
        for (j = 0; j < word_count; j++) {
            if (strcmp(words[j].text, current_word) == 0) {
                words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many unique words\n");
                break;
            }
            strncpy(words[word_count].text, current_word, sizeof(words[word_count].text) - 1);
            words[word_count].text[sizeof(words[word_count].text) - 1] = '\0';
            words[word_count].length = clean_len;
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (i = 0; i < word_count; i++) {
        printf("'%s': length %zu, count %d\n", 
               words[i].text, words[i].length, words[i].count);
    }
    
    if (word_count > 0) {
        size_t total_chars = 0;
        for (i = 0; i < word_count; i++) {
            total_chars += words[i].length * words[i].count;
        }
        printf("\nTotal characters in words: %zu\n", total_chars);
    }
    
    return 0;
}