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
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < max_len - 1) {
        if (isalnum((unsigned char)src[i])) {
            dest[j] = TO_LOWER(src[i]);
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

int process_text(const char *text, WordCount *words, int max_words) {
    char buffer[MAX_INPUT_LEN + 1];
    size_t text_len = strlen(text);
    if (text_len >= sizeof(buffer)) return 0;
    strcpy(buffer, text);
    
    int word_count = 0;
    char *token = buffer;
    char *end = buffer + text_len;
    
    while (token < end && word_count < max_words) {
        while (token < end && IS_WHITESPACE(*token)) token++;
        if (token >= end) break;
        
        char *word_start = token;
        while (token < end && !IS_WHITESPACE(*token) && !IS_TERMINATOR(*token)) token++;
        
        char original_word[MAX_WORD_LEN];
        size_t word_len = token - word_start;
        if (word_len >= sizeof(original_word)) continue;
        
        strncpy(original_word, word_start, word_len);
        original_word[word_len] = '\0';
        
        char normalized[MAX_WORD_LEN];
        normalize_word(normalized, original_word, sizeof(normalized));
        
        if (strlen(normalized) > 0) {
            int index = find_word_index(words, word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (strlen(normalized) < sizeof(words[word_count].word)) {
                    strcpy(words[word_count].word, normalized);
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        while (token < end && (IS_WHITESPACE(*token) || IS_TERMINATOR(*token))) token++;
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    int word_count = process_text(input, words, MAX_WORDS);
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found in input.\n