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

void process_string(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    int idx = 0;
    char current_word[MAX_WORD_LEN];
    int word_idx = 0;
    
    while (buffer[idx] != '\0' && *word_count < MAX_WORDS) {
        if (IS_WHITESPACE(buffer[idx])) {
            if (word_idx > 0) {
                current_word[word_idx] = '\0';
                
                for (int i = 0; current_word[i] != '\0'; i++) {
                    current_word[i] = TO_LOWER(current_word[i]);
                }
                
                int found_idx = find_word(words, *word_count, current_word);
                if (found_idx >= 0) {
                    if (words[found_idx].count < INT_MAX) {
                        words[found_idx].count++;
                    }
                } else {
                    if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_idx = 0;
            }
        } else if (isalpha((unsigned char)buffer[idx])) {
            if (word_idx < MAX_WORD_LEN - 1) {
                current_word[word_idx++] = buffer[idx];
            }
        }
        idx++;
    }
    
    if (word_idx > 0 && *word_count < MAX_WORDS) {
        current_word[word_idx] = '\0';
        for (int i = 0; current_word[i] != '\0'; i++) {
            current_word[i] = TO_LOWER(current_word[i]);
        }
        
        int found_idx = find_word(words, *word_count, current_word);
        if (found_idx >= 0) {
            if (words[found_idx].count < INT_MAX) {
                words[found_idx].count++;
            }
        } else {
            if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
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
    
    process_string(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return