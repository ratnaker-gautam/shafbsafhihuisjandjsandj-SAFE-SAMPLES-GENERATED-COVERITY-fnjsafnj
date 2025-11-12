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
    char word[MAX_WORD_LEN + 1];
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
    for (int i = 0; i < MAX_INPUT_LEN; i++) {
        if (str[i] == '\0') return 1;
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 0;
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
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0 || len >= MAX_WORD_LEN) return 0;
    strncpy(words[*count].word, word, MAX_WORD_LEN);
    words[*count].word[MAX_WORD_LEN] = '\0';
    words[*count].count = 1;
    (*count)++;
    return 1;
}

void process_input(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN + 1];
    if (!is_valid_input(input)) return;
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && *word_count < MAX_WORDS) {
        char normalized[MAX_WORD_LEN + 1];
        normalize_word(normalized, token, MAX_WORD_LEN);
        if (safe_strlen(normalized, MAX_WORD_LEN) > 0) {
            int index = find_word_index(words, *word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                add_word(words, word_count, normalized);
            }
        }
        token = strtok(NULL, " \t\n");
    }
}

void print_results(const WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
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
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
    } else {
        print_results(words, word_count);
    }
    
    return 0;
}