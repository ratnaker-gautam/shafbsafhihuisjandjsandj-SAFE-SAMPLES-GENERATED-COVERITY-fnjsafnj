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
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void process_word(WordCount *words, int *count, const char *word) {
    if (words == NULL || count == NULL || word == NULL || *count >= MAX_WORDS) return;
    
    char processed[MAX_WORD_LEN];
    size_t j = 0;
    for (size_t i = 0; word[i] != '\0' && j < MAX_WORD_LEN - 1; i++) {
        char c = TO_LOWER(word[i]);
        if (isalpha((unsigned char)c)) {
            processed[j++] = c;
        }
    }
    processed[j] = '\0';
    
    if (j == 0) return;
    
    int idx = find_word(words, *count, processed);
    if (idx >= 0) {
        if (words[idx].count < INT_MAX) words[idx].count++;
    } else {
        if (safe_strncpy(words[*count].word, processed, MAX_WORD_LEN) == 0) {
            words[*count].count = 1;
            (*count)++;
        }
    }
}

void tokenize_and_count(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    *word_count = 0;
    size_t len = strlen(input);
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, MAX_INPUT_LEN) != 0) return;
    
    size_t start = 0;
    while (start < len) {
        while (start < len && IS_WHITESPACE(buffer[start])) start++;
        if (start >= len) break;
        
        size_t end = start;
        while (end < len && !IS_WHITESPACE(buffer[end])) end++;
        
        if (end - start < MAX_WORD_LEN) {
            char word[MAX_WORD_LEN];
            size_t word_len = end - start;
            for (size_t i = 0; i < word_len && i < MAX_WORD_LEN - 1; i++) {
                word[i] = buffer[start + i];
            }
            word[word_len < MAX_WORD_LEN - 1 ? word_len : MAX_WORD_LEN - 1] = '\0';
            process_word(words, word_count, word);
        }
        
        start = end + 1;
    }
}

void print_word_counts(const WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    tokenize_and_count(input, words, &word_count);
    print_word_counts(words, word_count);
    
    return 0;
}