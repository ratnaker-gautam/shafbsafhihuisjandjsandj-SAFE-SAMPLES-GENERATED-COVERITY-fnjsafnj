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

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    size_t len = strlen(word);
    size_t write_idx = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = word[i];
        if (isalpha((unsigned char)c)) {
            word[write_idx++] = TO_LOWER(c);
        }
    }
    word[write_idx] = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    int count = 0;
    char *token = buffer;
    
    while (*token != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*token)) token++;
        if (*token == '\0') break;
        
        char *word_start = token;
        while (*token != '\0' && !IS_WHITESPACE(*token)) token++;
        
        char current_word[MAX_WORD_LEN];
        size_t word_len = token - word_start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        if (safe_strncpy(current_word, word_start, word_len + 1) != 0) continue;
        normalize_word(current_word);
        
        if (current_word[0] == '\0') continue;
        
        int idx = find_word_index(words, count, current_word);
        if (idx >= 0) {
            if (words[idx].count < INT_MAX) words[idx].count++;
        } else {
            if (safe_strncpy(words[count].word, current_word, MAX_WORD_LEN) == 0) {
                words[count].count = 1;
                count++;
            }
        }
        
        if (*token != '\0') token++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(WordCount *words, int count) {
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
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided\n");
        return 0;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}