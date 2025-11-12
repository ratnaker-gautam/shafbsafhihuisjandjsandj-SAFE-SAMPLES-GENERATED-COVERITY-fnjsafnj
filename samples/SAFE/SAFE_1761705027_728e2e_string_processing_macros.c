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
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words) {
    if (!is_valid_input(input)) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= sizeof(buffer)) return 0;
    memcpy(buffer, input, len);
    buffer[len] = '\0';
    
    to_lower_case(buffer);
    
    int word_count = 0;
    char *token = buffer;
    char *end = buffer + len;
    
    while (token < end && word_count < MAX_WORDS) {
        while (token < end && IS_WHITESPACE(*token)) token++;
        if (token >= end) break;
        
        char *word_start = token;
        while (token < end && !IS_WHITESPACE(*token) && !IS_TERMINATOR(*token)) token++;
        
        size_t word_len = token - word_start;
        if (word_len == 0) continue;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        char current_word[MAX_WORD_LEN];
        memcpy(current_word, word_start, word_len);
        current_word[word_len] = '\0';
        
        int existing_index = find_word(words, word_count, current_word);
        if (existing_index >= 0) {
            if (words[existing_index].count < INT_MAX) {
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
        
        while (token < end && (IS_WHITESPACE(*token) || IS_TERMINATOR(*token))) token++;
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
    WordCount words[MAX_WORDS] = {0};
    
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
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    int word_count = process_input(input, words);
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    return 0;
}