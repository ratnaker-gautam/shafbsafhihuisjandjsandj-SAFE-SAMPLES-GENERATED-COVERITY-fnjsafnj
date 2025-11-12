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

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, max_len);
    if (len >= max_len - 1) return 0;
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

int process_input(const char *input, WordCount *words, int max_words) {
    char buffer[MAX_INPUT_LEN];
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int word_count = 0;
    char *token = buffer;
    char *end = buffer;
    
    while (*token != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(*token)) {
            token++;
        }
        if (*token == '\0') break;
        
        end = token;
        while (*end != '\0' && !IS_WHITESPACE(*end)) {
            end++;
        }
        
        char temp_word[MAX_WORD_LEN];
        size_t token_len = (size_t)(end - token);
        if (token_len >= MAX_WORD_LEN) {
            token_len = MAX_WORD_LEN - 1;
        }
        
        strncpy(temp_word, token, token_len);
        temp_word[token_len] = '\0';
        
        char normalized[MAX_WORD_LEN];
        normalize_word(normalized, temp_word, MAX_WORD_LEN);
        
        if (normalized[0] != '\0') {
            int index = find_word_index(words, word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (word_count < max_words) {
                    strncpy(words[word_count].word, normalized, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = end;
        if (*token != '\0') {
            token++;
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found in input.\n");
    }
    
    return EXIT_SUCCESS;
}