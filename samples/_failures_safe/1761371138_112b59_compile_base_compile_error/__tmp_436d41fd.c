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

int process_text(const char *input, WordCount *words, int max_words) {
    if (!is_valid_input(input) || words == NULL || max_words <= 0) {
        return -1;
    }

    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= sizeof(buffer)) {
        return -1;
    }
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    int word_count = 0;
    size_t pos = 0;
    char current_word[MAX_WORD_LEN];

    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(buffer[pos])) {
            pos++;
        }

        if (pos >= input_len) break;

        size_t word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(buffer[pos]) && !IS_TERMINATOR(buffer[pos])) {
            pos++;
        }

        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            strncpy(current_word, &buffer[word_start], word_len);
            current_word[word_len] = '\0';
            
            char normalized[MAX_WORD_LEN];
            normalize_word(normalized, current_word, sizeof(normalized));
            
            if (normalized[0] != '\0') {
                int existing_index = find_word_index(words, word_count, normalized);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    if (safe_strlen(normalized, MAX_WORD_LEN) < MAX_WORD_LEN - 1) {
                        strncpy(words[word_count].word, normalized, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }

        while (pos < input_len && (IS_WHITESPACE(buffer[pos]) || IS_TERMINATOR(buffer[pos]))) {
            pos++;
        }
    }

    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (