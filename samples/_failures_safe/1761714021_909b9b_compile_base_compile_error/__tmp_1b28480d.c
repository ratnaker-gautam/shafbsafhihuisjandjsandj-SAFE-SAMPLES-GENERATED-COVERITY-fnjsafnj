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
    
    size_t len = strlen(buffer);
    if (len >= MAX_INPUT_LEN - 1) return -1;
    
    to_lower_case(buffer, len);
    
    int count = 0;
    size_t i = 0;
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && IS_WHITESPACE(buffer[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_valid_char(buffer[i])) i++;
        
        if (i > start) {
            size_t word_len = i - start;
            if (word_len >= MAX_WORD_LEN - 1) word_len = MAX_WORD_LEN - 2;
            
            char temp_word[MAX_WORD_LEN];
            size_t j;
            for (j = 0; j < word_len; j++) {
                temp_word[j] = buffer[start + j];
            }
            temp_word[j] = '\0';
            
            if (j > 0 && IS_TERMINATOR(temp_word[j-1])) {
                temp_word[j-1] = '\0';
                if (j-1 == 0) continue;
            }
            
            int found = -1;
            for (int k = 0; k < count; k++) {
                if (safe_strcmp(words[k].word, temp_word) == 0) {
                    found = k;
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
        }
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
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
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
    
    print_word_counts(words, word_count