//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -2;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    char *read = word;
    char *write = word;
    while (*read) {
        if (isalpha((unsigned char)*read)) {
            *write = TO_LOWER(*read);
            write++;
        }
        read++;
    }
    *write = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    char *token = buffer;
    int count = 0;
    
    while (*token && count < MAX_WORDS) {
        while (IS_WHITESPACE(*token)) token++;
        if (*token == '\0') break;
        
        char *start = token;
        while (*token && !IS_WHITESPACE(*token)) token++;
        
        size_t len = token - start;
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
        
        char temp_word[MAX_WORD_LEN];
        if (safe_strncpy(temp_word, start, len + 1) != 0) continue;
        temp_word[len] = '\0';
        
        normalize_word(temp_word);
        if (temp_word[0] == '\0') continue;
        
        int found = -1;
        for (int i = 0; i < count; i++) {
            if (safe_strcmp(words[i].word, temp_word) == 0) {
                found = i;
                break;
            }
        }
        
        if (found >= 0) {
            if (words[found].count < INT_MAX) {
                words[found].count++;
            }
        } else {
            if (safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN) == 0) {
                words[count].count = 1;
                count++;
            }
        }
        
        if (*token) token++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(const WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        printf("Word: %-20s Count: %d\n", words[i].word, words[i].count);
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
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    printf("\nWord frequency analysis:\n");
    print_word_counts(words, word_count);
    
    return 0;
}