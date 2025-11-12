//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    size_t count;
} WordCount;

size_t safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int safe_strncmp(const char *s1, const char *s2, size_t n) {
    if (n == 0) return 0;
    while (n-- > 0) {
        if (*s1 != *s2) {
            return (unsigned char)*s1 - (unsigned char)*s2;
        }
        if (*s1 == '\0') break;
        s1++;
        s2++;
    }
    return 0;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return;
    while (n-- > 0 && *src != '\0') {
        *dest++ = *src++;
    }
    *dest = '\0';
}

int is_valid_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, max_len);
    if (len == 0 || len >= max_len) return 0;
    return 1;
}

size_t tokenize_string(const char *input, char words[][MAX_WORD_LEN], size_t max_words) {
    if (!is_valid_input(input, MAX_INPUT_LEN)) return 0;
    
    size_t word_count = 0;
    const char *p = input;
    
    while (*p != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            safe_strncpy(words[word_count], start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    
    while (len > 0 && !isalnum((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
}

size_t count_unique_words(char words[][MAX_WORD_LEN], size_t word_count, WordCount *word_counts) {
    if (word_count == 0 || word_counts == NULL) return 0;
    
    size_t unique_count = 0;
    
    for (size_t i = 0; i < word_count; i++) {
        char normalized[MAX_WORD_LEN];
        safe_strncpy(normalized, words[i], MAX_WORD_LEN);
        normalize_word(normalized);
        
        if (safe_strlen(normalized, MAX_WORD_LEN) == 0) continue;
        
        int found = 0;
        for (size_t j = 0; j < unique_count; j++) {
            if (safe_strncmp(word_counts[j].word, normalized, MAX_WORD_LEN) == 0) {
                if (word_counts[j].count < SIZE_MAX) {
                    word_counts[j].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            safe_strncpy(word_counts[unique_count].word, normalized, MAX_WORD_LEN);
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return safe_strncmp(wa->word, wb->word, MAX_WORD_LEN);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");