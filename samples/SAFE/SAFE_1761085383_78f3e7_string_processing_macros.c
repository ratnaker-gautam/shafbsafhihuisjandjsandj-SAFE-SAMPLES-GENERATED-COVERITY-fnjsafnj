//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i < input_len && *word_count < MAX_WORDS; ) {
        while (i < input_len && IS_WHITESPACE(input[i])) i++;
        if (i >= input_len) break;
        
        size_t word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len == 0) continue;
        
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        for (size_t j = 0; j < word_len; j++) {
            current_word[j] = tolower(input[word_start + j]);
        }
        current_word[word_len] = '\0';
        
        int found = 0;
        for (int j = 0; j < *word_count; j++) {
            if (strcmp(words[j].word, current_word) == 0) {
                if (words[j].count < INT_MAX) {
                    words[j].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
        
        while (i < input_len && (IS_WHITESPACE(input[i]) || IS_TERMINATOR(input[i]))) {
            i++;
        }
    }
    
    return 1;
}

void print_word_counts(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}