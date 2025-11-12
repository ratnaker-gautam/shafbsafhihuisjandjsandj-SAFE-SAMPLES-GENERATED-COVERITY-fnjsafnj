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

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        char c = src[i];
        if (isalpha((unsigned char)c)) {
            dest[i] = TO_LOWER(c);
        } else {
            dest[i] = c;
        }
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

int add_word(WordCount *words, int *count, const char *word) {
    if (*count >= MAX_WORDS) return 0;
    
    char normalized[MAX_WORD_LEN];
    normalize_word(normalized, word, MAX_WORD_LEN);
    
    if (safe_strlen(normalized, MAX_WORD_LEN) == 0) return 0;
    
    int index = find_word_index(words, *count, normalized);
    if (index >= 0) {
        words[index].count++;
        return 1;
    }
    
    size_t len = safe_strlen(normalized, MAX_WORD_LEN);
    if (len >= MAX_WORD_LEN) return 0;
    
    strncpy(words[*count].word, normalized, MAX_WORD_LEN - 1);
    words[*count].word[MAX_WORD_LEN - 1] = '\0';
    words[*count].count = 1;
    (*count)++;
    return 1;
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    *word_count = 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        if (IS_WHITESPACE(c) || c == '\0' || !isalpha((unsigned char)c)) {
            if (buf_index > 0) {
                buffer[buf_index] = '\0';
                if (!add_word(words, word_count, buffer)) {
                    return 0;
                }
                buf_index = 0;
            }
        } else {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = c;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

void print_word_stats(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    printf("Total unique words: %d\n", count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
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