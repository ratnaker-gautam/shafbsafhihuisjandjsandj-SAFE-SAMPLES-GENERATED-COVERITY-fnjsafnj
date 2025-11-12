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

int process_input(const char *input, WordCount *words, int max_words) {
    if (!is_valid_input(input) || words == NULL || max_words <= 0) {
        return 0;
    }

    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= sizeof(buffer)) {
        return 0;
    }
    memcpy(buffer, input, input_len);
    buffer[input_len] = '\0';

    int word_count = 0;
    char *token = buffer;
    char *end = buffer + input_len;

    while (token < end && word_count < max_words) {
        while (token < end && IS_WHITESPACE(*token)) {
            token++;
        }
        if (token >= end) break;

        char *word_start = token;
        while (token < end && !IS_WHITESPACE(*token)) {
            token++;
        }
        char *word_end = token;

        size_t word_len = word_end - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char normalized[MAX_WORD_LEN];
            char temp_word[MAX_WORD_LEN];
            if (word_len >= sizeof(temp_word)) {
                continue;
            }
            memcpy(temp_word, word_start, word_len);
            temp_word[word_len] = '\0';
            normalize_word(normalized, temp_word, sizeof(normalized));

            if (normalized[0] != '\0') {
                int index = find_word_index(words, word_count, normalized);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    size_t norm_len = safe_strlen(normalized, sizeof(words[word_count].word));
                    if (norm_len < sizeof(words[word_count].word)) {
                        memcpy(words[word_count].word, normalized, norm_len + 1);
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
    }
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        printf("Word: %s, Count: %d\n", words[i].word, words[i].count);
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

    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    int word_count = process_input(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }

    printf("Word frequencies:\n");
    print_word_counts(words, word_count);

    return 0;
}