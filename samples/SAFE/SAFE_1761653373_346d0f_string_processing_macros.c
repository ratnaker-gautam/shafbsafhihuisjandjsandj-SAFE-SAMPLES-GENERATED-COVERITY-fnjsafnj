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
    return len > 0 && len <= MAX_INPUT_LEN;
}

void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

void normalize_word(char *word) {
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    if (len == 0) return;
    
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_pos++] = TO_LOWER(word[i]);
        }
    }
    word[write_pos] = '\0';
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
    safe_strncpy(buffer, input, sizeof(buffer));
    
    int count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && count < MAX_WORDS) {
        char normalized[MAX_WORD_LEN];
        safe_strncpy(normalized, token, sizeof(normalized));
        normalize_word(normalized);
        
        if (safe_strlen(normalized, MAX_WORD_LEN) > 0) {
            int index = find_word_index(words, *word_count, normalized);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else if (*word_count < MAX_WORDS) {
                safe_strncpy(words[*word_count].word, normalized, MAX_WORD_LEN);
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
        
        token = strtok(NULL, " \t\n");
        count++;
    }
    
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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found in input\n");
    }
    
    return 0;
}