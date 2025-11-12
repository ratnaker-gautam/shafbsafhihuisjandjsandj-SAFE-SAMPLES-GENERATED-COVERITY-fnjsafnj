//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -1;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void to_lower_case(char *str, size_t len) {
    if (str == NULL) return;
    for (size_t i = 0; i < len && str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    to_lower_case(buffer, sizeof(buffer));
    
    *word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && *word_count < MAX_WORDS) {
        while (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && !IS_WHITESPACE(buffer[i]) && 
               !IS_TERMINATOR(buffer[i]) && is_valid_char(buffer[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char current_word[MAX_WORD_LEN];
            if (safe_strncpy(current_word, &buffer[start], word_len + 1) != 0) continue;
            current_word[word_len] = '\0';
            
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (safe_strcmp(words[j].word, current_word) == 0) {
                    if (words[j].count < INT_MAX) {
                        words[j].count++;
                    }
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[*word_count].word, current_word, sizeof(words[*word_count].word)) == 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
        
        while (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i])) {
            i++;
        }
    }
    
    return 0;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
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
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return EXIT_SUCCESS;
    }
    
    print_word_counts(words, word