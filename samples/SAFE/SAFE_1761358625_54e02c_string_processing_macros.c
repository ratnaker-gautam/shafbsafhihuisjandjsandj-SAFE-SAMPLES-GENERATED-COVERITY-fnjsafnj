//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    size_t write_pos = 0;
    
    for (size_t i = 0; i < len && write_pos < MAX_WORD_LEN - 1; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_pos++] = TO_LOWER(word[i]);
        }
    }
    word[write_pos] = '\0';
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
    if (!is_valid_input(input) || words == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    if (input_len >= sizeof(buffer)) return 0;
    memcpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    
    int word_count = 0;
    char *token = buffer;
    
    while (*token != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(*token)) token++;
        if (*token == '\0') break;
        
        char *word_start = token;
        while (*token != '\0' && !IS_WHITESPACE(*token)) {
            token++;
        }
        
        char current_word[MAX_WORD_LEN];
        size_t word_len = token - word_start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        memcpy(current_word, word_start, word_len);
        current_word[word_len] = '\0';
        normalize_word(current_word);
        
        if (current_word[0] != '\0') {
            int existing_index = find_word_index(words, word_count, current_word);
            if (existing_index >= 0) {
                if (words[existing_index].count < INT_MAX) {
                    words[existing_index].count++;
                }
            } else {
                size_t normalized_len = safe_strlen(current_word, MAX_WORD_LEN);
                if (normalized_len > 0 && normalized_len < MAX_WORD_LEN) {
                    memcpy(words[word_count].word, current_word, normalized_len + 1);
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        if (*token != '\0') token++;
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
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
    
    int word_count = process_input(input, words);
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found in input.\n");
    }
    
    return 0;
}