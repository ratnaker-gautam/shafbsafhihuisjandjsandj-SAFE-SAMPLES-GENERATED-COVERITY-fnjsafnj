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

int process_word(const char *word, WordCount *words, int *word_count) {
    if (word == NULL || words == NULL || word_count == NULL) return -1;
    if (*word_count >= MAX_WORDS) return -1;
    
    char normalized[MAX_WORD_LEN];
    size_t i, j = 0;
    for (i = 0; word[i] != '\0' && j < MAX_WORD_LEN - 1; i++) {
        char c = word[i];
        if (isalnum((unsigned char)c)) {
            normalized[j++] = TO_LOWER(c);
        }
    }
    normalized[j] = '\0';
    
    if (j == 0) return 0;
    
    for (int idx = 0; idx < *word_count; idx++) {
        if (strcmp(words[idx].word, normalized) == 0) {
            if (words[idx].count < INT_MAX) {
                words[idx].count++;
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
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return -1;
    
    *word_count = 0;
    const char *start = input;
    const char *end = input;
    
    while (*start != '\0') {
        while (IS_WHITESPACE(*start)) start++;
        if (*start == '\0') break;
        
        end = start;
        while (*end != '\0' && !IS_WHITESPACE(*end)) end++;
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, start, word_len + 1) == 0) {
                if (process_word(temp_word, words, word_count) != 0) break;
            }
        }
        
        start = end;
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d chars): ", MAX_INPUT_LEN - 1);
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
    
    if (tokenize_and_count(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i <