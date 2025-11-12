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
    const char *p = input;
    
    while (*p != '\0') {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p)) p++;
        
        size_t len = p - start;
        if (len > 0 && len < MAX_WORD_LEN) {
            char temp[MAX_WORD_LEN];
            if (safe_strncpy(temp, start, len + 1) == 0) {
                process_word(words, word_count, temp);
            }
        }
    }
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
    
    printf("Enter text for word frequency analysis: ");
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
    
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    tokenize_and_count(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found in input.\n");