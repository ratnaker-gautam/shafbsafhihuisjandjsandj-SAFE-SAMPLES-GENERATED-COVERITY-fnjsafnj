//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
}

int safe_strcmp(const char *s1, const char *s2, size_t max_len) {
    for (size_t i = 0; i < max_len; i++) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        if (s1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i, j = 0;
    for (i = 0; src[i] != '\0' && i < max_len - 1; i++) {
        if (IS_ALPHA(src[i])) {
            dest[j++] = TO_LOWER(src[i]);
        }
    }
    dest[j] = '\0';
}

int process_input(const char *input, WordCount *words, int max_words) {
    char normalized[MAX_WORD_LEN];
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char temp_word[MAX_WORD_LEN];
        safe_strncpy(temp_word, &input[start], word_len + 1);
        normalize_word(normalized, temp_word, MAX_WORD_LEN);
        
        if (normalized[0] != '\0') {
            int found = 0;
            for (int j = 0; j < word_count; j++) {
                if (safe_strcmp(words[j].word, normalized, MAX_WORD_LEN) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                safe_strncpy(words[word_count].word, normalized, MAX_WORD_LEN);
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (safe_strcmp(words[j].word, words[j + 1].word, MAX_WORD_LEN) > 0) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (safe_strlen(input, MAX_INPUT_LEN) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequencies (alphabetical order):\n");
    for