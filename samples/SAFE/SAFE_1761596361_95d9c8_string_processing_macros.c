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

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    to_lower_case(buffer);
    
    int count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && count < MAX_WORDS) {
        while (buffer[i] != '\0' && !is_valid_char(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && is_valid_char(buffer[i])) {
            i++;
        }
        
        size_t len = i - start;
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
        
        char temp[MAX_WORD_LEN];
        for (size_t j = 0; j < len; j++) {
            temp[j] = buffer[start + j];
        }
        temp[len] = '\0';
        
        int found = -1;
        for (int j = 0; j < count; j++) {
            if (safe_strcmp(words[j].word, temp) == 0) {
                found = j;
                break;
            }
        }
        
        if (found >= 0) {
            if (words[found].count < INT_MAX) {
                words[found].count++;
            }
        } else {
            if (safe_strncpy(words[count].word, temp, sizeof(words[count].word)) == 0) {
                words[count].count = 1;
                count++;
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}