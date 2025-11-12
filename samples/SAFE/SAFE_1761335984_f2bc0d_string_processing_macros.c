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
    
    int len = strlen(word);
    int start = 0;
    int end = len - 1;
    
    while (start < len && !isalnum((unsigned char)word[start])) start++;
    while (end >= 0 && !isalnum((unsigned char)word[end])) end--;
    
    if (start > end) {
        word[0] = '\0';
        return;
    }
    
    int new_len = end - start + 1;
    for (int i = 0; i < new_len; i++) {
        word[i] = TO_LOWER(word[start + i]);
    }
    word[new_len] = '\0';
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return 0;
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && word_count < max_words) {
        char normalized[MAX_WORD_LEN];
        if (safe_strncpy(normalized, token, sizeof(normalized)) != 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }
        
        normalize_word(normalized);
        
        if (normalized[0] != '\0') {
            int index = find_word_index(words, word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (safe_strncpy(words[word_count].word, normalized, sizeof(words[word_count].word)) == 0) {
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    return word_count;
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
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found\n");
    }
    
    return 0;
}