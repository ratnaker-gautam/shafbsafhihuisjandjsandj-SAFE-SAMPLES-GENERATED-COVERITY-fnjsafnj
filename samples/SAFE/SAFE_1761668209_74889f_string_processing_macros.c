//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50
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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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
    
    int count = 0;
    char *token = buffer;
    char *end = buffer + input_len;
    
    while (token < end && count < MAX_WORDS) {
        while (token < end && IS_WHITESPACE(*token)) {
            token++;
        }
        if (token >= end) break;
        
        char *word_start = token;
        while (token < end && !IS_WHITESPACE(*token)) {
            token++;
        }
        
        char temp_word[MAX_WORD_LEN];
        size_t word_len = token - word_start;
        if (word_len >= sizeof(temp_word)) continue;
        
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        char normalized[MAX_WORD_LEN];
        normalize_word(normalized, temp_word, sizeof(normalized));
        
        if (safe_strlen(normalized, sizeof(normalized)) > 0) {
            int index = find_word_index(words, *word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else if (*word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, normalized, sizeof(words[*word_count].word) - 1);
                words[*word_count].word[sizeof(words[*word_count].word) - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    return 1;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        if (safe_strlen(words[i].word, sizeof(words[i].word)) > 0) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Invalid input or processing error\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
    } else {
        print_word_counts(words, word_count);
    }
    
    return 0;
}