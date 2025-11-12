//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    while (i < max_len - 1 && src[i] != '\0') {
        char c = src[i];
        if (isalpha((unsigned char)c)) {
            dest[i] = TO_LOWER(c);
        } else {
            dest[i] = c;
        }
        i++;
    }
    dest[i] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (!is_valid_input(input)) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= sizeof(buffer)) return 0;
    memcpy(buffer, input, len);
    buffer[len] = '\0';
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && word_count < max_words) {
        char normalized[MAX_WORD_LEN];
        normalize_word(normalized, token, sizeof(normalized));
        
        if (safe_strlen(normalized, MAX_WORD_LEN) > 0) {
            int index = find_word_index(words, word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (safe_strlen(normalized, MAX_WORD_LEN) < MAX_WORD_LEN) {
                    strcpy(words[word_count].word, normalized);
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    return word_count;
}

void print_results(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d chars):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count > 0) {
        print_results(words, word_count);
    } else {
        printf("No valid words found or input error\n");
    }
    
    return 0;
}