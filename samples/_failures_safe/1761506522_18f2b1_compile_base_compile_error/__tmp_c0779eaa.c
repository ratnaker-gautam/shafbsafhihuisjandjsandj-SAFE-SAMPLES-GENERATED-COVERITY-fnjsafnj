//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

typedef struct {
    char word[MAX_WORD_LEN + 1];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int is_valid_input(const char *str) {
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *word) {
    size_t len = safe_strlen(word, MAX_WORD_LEN + 1);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
}

int add_word(WordCount *words, int *word_count, const char *word) {
    if (*word_count >= MAX_WORDS) {
        return 0;
    }
    
    size_t len = safe_strlen(word, MAX_WORD_LEN + 1);
    if (len == 0 || len > MAX_WORD_LEN) {
        return 0;
    }
    
    char normalized[MAX_WORD_LEN + 1];
    safe_strncpy(normalized, word, sizeof(normalized));
    normalize_word(normalized);
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, normalized) == 0) {
            words[i].count++;
            return 1;
        }
    }
    
    safe_strncpy(words[*word_count].word, normalized, sizeof(words[*word_count].word));
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

int extract_words(const char *input, WordCount *words, int *word_count) {
    *word_count = 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len == 0) return 1;
    
    char buffer[MAX_INPUT_LEN + 1];
    safe_strncpy(buffer, input, sizeof(buffer));
    
    char *token = buffer;
    char *end = buffer + len;
    
    while (token < end && *word_count < MAX_WORDS) {
        while (token < end && IS_WHITESPACE(*token)) {
            token++;
        }
        
        if (token >= end) break;
        
        char *word_start = token;
        while (token < end && !IS_WHITESPACE(*token)) {
            token++;
        }
        
        size_t word_len = token - word_start;
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN + 1];
            safe_strncpy(temp_word, word_start, word_len + 1);
            temp_word[word_len] = '\0';
            
            if (!add_word(words, word_count, temp_word)) {
                return 0;
            }
        }
    }
    
    return 1;
}

void print_word_counts(const WordCount *words, int word_count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    if (!extract_words(input