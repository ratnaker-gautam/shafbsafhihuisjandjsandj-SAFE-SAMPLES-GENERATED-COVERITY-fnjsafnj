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

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void process_word(WordCount *words, int *count, const char *word) {
    if (words == NULL || count == NULL || word == NULL || *count >= MAX_WORDS) return;
    
    char normalized[MAX_WORD_LEN];
    size_t i;
    for (i = 0; i < MAX_WORD_LEN - 1 && word[i] != '\0'; i++) {
        normalized[i] = TO_LOWER((unsigned char)word[i]);
    }
    normalized[i] = '\0';
    
    if (strlen(normalized) == 0) return;
    
    int idx = find_word(words, *count, normalized);
    if (idx >= 0) {
        if (words[idx].count < INT_MAX) words[idx].count++;
    } else {
        if (safe_strncpy(words[*count].word, normalized, MAX_WORD_LEN) == 0) {
            words[*count].count = 1;
            (*count)++;
        }
    }
}

int tokenize_and_count(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    if (input_len == 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, MAX_INPUT_LEN) != 0) return -1;
    
    size_t start = 0;
    size_t end = 0;
    
    while (end < input_len && *word_count < MAX_WORDS) {
        while (start < input_len && IS_WHITESPACE(buffer[start])) start++;
        if (start >= input_len) break;
        
        end = start;
        while (end < input_len && !IS_WHITESPACE(buffer[end])) end++;
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, &buffer[start], word_len + 1) == 0) {
                process_word(words, word_count, temp_word);
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
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') input[len - 1] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (tokenize_and_count(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0