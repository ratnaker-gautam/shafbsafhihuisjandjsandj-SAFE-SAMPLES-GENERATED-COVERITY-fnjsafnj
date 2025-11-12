//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    size_t write_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_idx++] = TO_LOWER(word[i]);
        }
    }
    word[write_idx] = '\0';
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
    if (!is_valid_input(input) || words == NULL || max_words <= 0) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && word_count < max_words) {
        char normalized[MAX_WORD_LEN];
        if (safe_strncpy(normalized, token, sizeof(normalized)) != 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }
        
        normalize_word(normalized);
        if (strlen(normalized) == 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }
        
        int idx = find_word_index(words, word_count, normalized);
        if (idx >= 0) {
            if (words[idx].count < INT_MAX) {
                words[idx].count++;
            }
        } else {
            if (safe_strncpy(words[word_count].word, normalized, sizeof(words[word_count].word)) == 0) {
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    printf("\nWord frequency analysis:\n");
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}