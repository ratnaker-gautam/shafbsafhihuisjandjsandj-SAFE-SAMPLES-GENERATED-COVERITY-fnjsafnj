//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
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
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '\0') break;
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_pos++] = word[i];
        }
    }
    word[write_pos] = '\0';
}

int add_word(WordCount *words, int *word_count, const char *word) {
    if (words == NULL || word_count == NULL || *word_count < 0) return 0;
    if (*word_count >= MAX_WORDS) return 0;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 1;
        }
    }
    
    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) {
        return 0;
    }
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    if (input_len >= sizeof(buffer)) return 0;
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    size_t pos = 0;
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    while (pos < input_len && buffer[pos] != '\0') {
        if (IS_WHITESPACE(buffer[pos]) || IS_TERMINATOR(buffer[pos])) {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                normalize_word(current_word);
                if (current_word[0] != '\0') {
                    if (!add_word(words, word_count, current_word)) {
                        return 0;
                    }
                }
                word_pos = 0;
            }
        } else {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = buffer[pos];
            } else {
                return 0;
            }
        }
        pos++;
    }
    
    if (word_pos > 0) {
        current_word[word_pos] = '\0';
        normalize_word(current_word);
        if (current_word[0] != '\0') {
            if (!add_word(words, word_count, current_word)) {
                return 0;
            }
        }
    }
    
    return 1;
}

void print_word_stats(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count