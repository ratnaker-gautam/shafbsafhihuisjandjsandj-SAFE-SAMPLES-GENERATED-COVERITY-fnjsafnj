//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -2;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int tokenize_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    to_lower_case(buffer);
    
    *word_count = 0;
    size_t len = strlen(buffer);
    size_t i = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        while (i < len && IS_WHITESPACE(buffer[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_valid_char(buffer[i])) i++;
        
        if (i > start) {
            size_t word_len = i - start;
            if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
            
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, &buffer[start], word_len + 1) != 0) continue;
            
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (safe_strcmp(words[j].word, temp_word) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[*word_count].word, temp_word, sizeof(words[*word_count].word)) == 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
        
        while (i < len && !IS_WHITESPACE(buffer[i]) && !is_valid_char(buffer[i])) i++;
    }
    
    return 0;
}

void sort_words(WordCount *words, int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (safe_strcmp(words[j].word, words[j + 1].word) > 0) {
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
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\0')) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (tokenize_input(input, words, &word_count) != 0) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %