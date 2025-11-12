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

int process_word(const char *word, WordCount *words, int *word_count) {
    if (word == NULL || words == NULL || word_count == NULL) return -1;
    if (*word_count >= MAX_WORDS) return -1;
    
    char normalized[MAX_WORD_LEN];
    size_t i, j = 0;
    for (i = 0; word[i] != '\0' && j < MAX_WORD_LEN - 1; i++) {
        char c = TO_LOWER(word[i]);
        if (isalpha((unsigned char)c)) {
            normalized[j++] = c;
        }
    }
    normalized[j] = '\0';
    
    if (j == 0) return 0;
    
    for (int k = 0; k < *word_count; k++) {
        if (strcmp(words[k].word, normalized) == 0) {
            if (words[k].count < INT_MAX) {
                words[k].count++;
            }
            return 0;
        }
    }
    
    if (safe_strncpy(words[*word_count].word, normalized, MAX_WORD_LEN) == 0) {
        words[*word_count].count = 1;
        (*word_count)++;
        return 0;
    }
    return -1;
}

int tokenize_and_count(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, MAX_INPUT_LEN) != 0) return -1;
    
    *word_count = 0;
    char *token = buffer;
    int in_word = 0;
    
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (IS_WHITESPACE(buffer[i])) {
            if (in_word) {
                buffer[i] = '\0';
                if (process_word(token, words, word_count) != 0) break;
                in_word = 0;
            }
        } else {
            if (!in_word) {
                token = &buffer[i];
                in_word = 1;
            }
        }
    }
    
    if (in_word) {
        if (process_word(token, words, word_count) != 0) return -1;
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        if (wb->count > wa->count) return 1;
        if (wb->count < wa->count) return -1;
        return 0;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
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
    
    if (tokenize_and_count(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return EXIT_SUCCESS;
    }
    
    qsort(words, (size_t)word_count, sizeof(Word