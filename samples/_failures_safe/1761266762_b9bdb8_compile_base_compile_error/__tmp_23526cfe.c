//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void normalize_word(char *dest, const char *src, size_t max_len) {
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < max_len - 1) {
        if (isalnum((unsigned char)src[i])) {
            dest[j] = TO_LOWER(src[i]);
            j++;
        }
        i++;
    }
    dest[j] = '\0';
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= sizeof(buffer)) return 0;
    strncpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    
    char *token = buffer;
    int count = *word_count;
    
    while (*token != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*token)) token++;
        if (*token == '\0') break;
        
        char *start = token;
        while (*token != '\0' && !IS_WHITESPACE(*token)) token++;
        char *end = token;
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (word_len >= sizeof(temp_word)) continue;
            strncpy(temp_word, start, word_len);
            temp_word[word_len] = '\0';
            
            char normalized[MAX_WORD_LEN];
            normalize_word(normalized, temp_word, sizeof(normalized));
            
            if (normalized[0] != '\0') {
                int index = find_word_index(words, count, normalized);
                if (index >= 0) {
                    if (words[index].count < INT_MAX) {
                        words[index].count++;
                    }
                } else {
                    if (safe_strlen(normalized, MAX_WORD_LEN) < MAX_WORD_LEN - 1) {
                        strncpy(words[count].word, normalized, MAX_WORD_LEN - 1);
                        words[count].word[MAX_WORD_LEN - 1] = '\0';
                        words[count].count = 1;
                        count++;
                    }
                }
            }
        }
        
        if (*token != '\0') token++;
    }
    
    *word_count = count;
    return 1;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
    } else {
        print_word_counts(words, word