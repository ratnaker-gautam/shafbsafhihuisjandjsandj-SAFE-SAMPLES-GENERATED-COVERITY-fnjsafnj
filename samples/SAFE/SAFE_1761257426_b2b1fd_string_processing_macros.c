//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    size_t write_idx = 0;
    
    for (size_t i = 0; i < len && write_idx < MAX_WORD_LEN - 1; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_idx++] = TO_LOWER(word[i]);
        }
    }
    word[write_idx] = '\0';
}

int add_word(WordCount *words, int *word_count, const char *word) {
    if (*word_count >= MAX_WORDS) return 0;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    
    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

void process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return;
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char current_word[MAX_WORD_LEN];
    size_t word_idx = 0;
    size_t input_len = safe_strlen(buffer, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !IS_WHITESPACE(buffer[i])) {
            if (word_idx < MAX_WORD_LEN - 1) {
                current_word[word_idx++] = buffer[i];
            }
        } else {
            if (word_idx > 0) {
                current_word[word_idx] = '\0';
                normalize_word(current_word);
                if (current_word[0] != '\0') {
                    add_word(words, word_count, current_word);
                }
                word_idx = 0;
            }
        }
    }
}

void print_word_counts(const WordCount *words, int word_count) {
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
    
    process_input(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found\n");
    } else {
        print_word_counts(words, word_count);
    }
    
    return 0;
}