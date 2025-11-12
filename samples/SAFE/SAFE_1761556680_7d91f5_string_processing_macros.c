//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int is_alpha_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < max_len - 1) {
        char c = TO_LOWER(src[i]);
        if (is_alpha_char(c)) {
            dest[j] = c;
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int max_words) {
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t pos = 0;
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        size_t start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        size_t word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char raw_word[MAX_WORD_LEN];
            char normalized[MAX_WORD_LEN];
            
            size_t copy_len = word_len;
            if (copy_len >= MAX_WORD_LEN) {
                copy_len = MAX_WORD_LEN - 1;
            }
            
            memcpy(raw_word, &input[start], copy_len);
            raw_word[copy_len] = '\0';
            
            normalize_word(normalized, raw_word, MAX_WORD_LEN);
            
            if (normalized[0] != '\0') {
                int existing_index = find_word_index(words, word_count, normalized);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    strncpy(words[word_count].word, normalized, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (safe_strlen(input, MAX_INPUT_LEN) == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
    } else {
        print_word_counts(words, word_count);
    }
    
    return 0;
}