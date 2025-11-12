//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        char c = src[i];
        if (IS_TERMINATOR(c) || IS_WHITESPACE(c)) {
            break;
        }
        dest[i] = TO_LOWER(c);
        i++;
    }
    dest[i] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t pos = 0;
    
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        size_t word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && !IS_TERMINATOR(input[pos])) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char temp_word[MAX_WORD_LEN];
            size_t copy_len = word_len;
            if (copy_len >= MAX_WORD_LEN) {
                copy_len = MAX_WORD_LEN - 1;
            }
            
            for (size_t i = 0; i < copy_len; i++) {
                temp_word[i] = input[word_start + i];
            }
            temp_word[copy_len] = '\0';
            
            char normalized[MAX_WORD_LEN];
            normalize_word(normalized, temp_word, MAX_WORD_LEN);
            
            if (normalized[0] != '\0') {
                int existing_index = find_word_index(words, word_count, normalized);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    size_t norm_len = safe_strlen(normalized, MAX_WORD_LEN);
                    if (norm_len > 0) {
                        for (size_t i = 0; i < norm_len && i < MAX_WORD_LEN - 1; i++) {
                            words[word_count].word[i] = normalized[i];
                        }
                        words[word_count].word[norm_len] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
        
        while (pos < input_len && IS_TERMINATOR(input[pos])) {
            pos++;
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("'%s': %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }