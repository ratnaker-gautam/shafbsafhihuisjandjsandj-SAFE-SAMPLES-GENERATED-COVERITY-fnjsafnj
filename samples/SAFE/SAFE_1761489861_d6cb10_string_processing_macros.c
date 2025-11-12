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
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int safe_strncpy(char *dest, const char *src, size_t dest_size) {
    if (dest == NULL || src == NULL || dest_size == 0) {
        return 0;
    }
    
    size_t i;
    for (i = 0; i < dest_size - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_valid_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void normalize_word(char *word) {
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_pos++] = TO_LOWER(word[i]);
        }
    }
    word[write_pos] = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) {
        return 0;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) {
        return 0;
    }
    
    int count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && count < MAX_WORDS) {
        char normalized[MAX_WORD_LEN];
        if (!safe_strncpy(normalized, token, sizeof(normalized))) {
            token = strtok(NULL, " \t\n");
            continue;
        }
        
        normalize_word(normalized);
        
        if (safe_strlen(normalized, MAX_WORD_LEN) > 0) {
            int existing_index = find_word(words, count, normalized);
            if (existing_index >= 0) {
                if (words[existing_index].count < INT_MAX) {
                    words[existing_index].count++;
                }
            } else {
                if (safe_strncpy(words[count].word, normalized, sizeof(words[count].word))) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    *word_count = count;
    return 1;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        printf("Word: %-20s Count: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    printf("\nWord frequency analysis:\n");
    print_word_counts(words, word_count);
    
    return 0;
}