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
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

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

int safe_tolower(int c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

int process_word(const char *word, WordCount *words, int *word_count) {
    if (word == NULL || words == NULL || word_count == NULL) return -1;
    if (*word_count >= MAX_WORDS) return -1;
    
    char clean_word[MAX_WORD_LEN];
    size_t j = 0;
    for (size_t i = 0; word[i] != '\0' && j < MAX_WORD_LEN - 1; i++) {
        char c = (char)safe_tolower((unsigned char)word[i]);
        if (isalpha((unsigned char)c)) {
            clean_word[j++] = c;
        }
    }
    clean_word[j] = '\0';
    
    if (j == 0) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, clean_word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 0;
        }
    }
    
    if (safe_strncpy(words[*word_count].word, clean_word, MAX_WORD_LEN) == 0) {
        words[*word_count].count = 1;
        (*word_count)++;
        return 0;
    }
    
    return -1;
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, MAX_INPUT_LEN) != 0) return -1;
    
    *word_count = 0;
    char *start = buffer;
    char *end = buffer;
    
    while (*start != '\0') {
        while (IS_WHITESPACE(*start)) start++;
        if (*start == '\0') break;
        
        end = start;
        while (*end != '\0' && !IS_WHITESPACE(*end) && !IS_TERMINATOR(*end)) end++;
        
        char temp = *end;
        *end = '\0';
        
        if (process_word(start, words, word_count) != 0) {
            *end = temp;
            break;
        }
        
        *end = temp;
        start = end;
        if (*start != '\0') start++;
    }
    
    return 0;
}

void print_word_stats(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    if (extract_words(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing words\n");
        return EXIT_FAILURE;