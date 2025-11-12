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
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

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
    if (words == NULL || word == NULL || count <= 0) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    int len = strlen(word);
    int write_pos = 0;
    
    for (int i = 0; i < len; i++) {
        if (IS_ALPHA(word[i])) {
            word[write_pos++] = TO_LOWER(word[i]);
        }
    }
    word[write_pos] = '\0';
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return 0;
    
    int word_count = 0;
    char *ptr = buffer;
    
    while (*ptr != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        char current_word[MAX_WORD_LEN];
        int word_len = 0;
        
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr) && word_len < MAX_WORD_LEN - 1) {
            current_word[word_len++] = *ptr++;
        }
        current_word[word_len] = '\0';
        
        if (word_len > 0) {
            normalize_word(current_word);
            if (current_word[0] != '\0') {
                int existing_index = find_word_index(words, word_count, current_word);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    if (safe_strncpy(words[word_count].word, current_word, MAX_WORD_LEN) == 0) {
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
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
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nWord frequency analysis:\n");
    print_word_counts(words, word_count);
    
    return 0;
}