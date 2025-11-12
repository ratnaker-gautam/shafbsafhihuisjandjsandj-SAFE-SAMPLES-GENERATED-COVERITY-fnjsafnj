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
        if (IS_ALPHA(*read)) {
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
    
    int count = 0;
    char *token = buffer;
    char *context = NULL;
    
    while (*token != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*token)) token++;
        if (*token == '\0') break;
        
        char *word_start = token;
        while (*token != '\0' && !IS_WHITESPACE(*token)) token++;
        
        char current_word[MAX_WORD_LEN];
        size_t len = token - word_start;
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
        
        if (safe_strncpy(current_word, word_start, len + 1) != 0) continue;
        current_word[len] = '\0';
        
        normalize_word(current_word);
        if (current_word[0] == '\0') continue;
        
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (safe_strcmp(words[i].word, current_word) == 0) {
                if (words[i].count < INT_MAX) words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && count < MAX_WORDS) {
            if (safe_strncpy(words[count].word, current_word, MAX_WORD_LEN) == 0) {
                words[count].count = 1;
                count++;
            }
        }
        
        if (*token == '\0') break;
        token++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(const WordCount *words, int count) {
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
        printf("Error reading input\n");
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
        printf("Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}