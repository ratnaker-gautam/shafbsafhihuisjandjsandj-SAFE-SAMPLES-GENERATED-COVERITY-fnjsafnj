//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) {
        return 0;
    }
    for (size_t i = 0; i < max_len && str[i] != '\0'; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

int normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') {
        return 0;
    }
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    return 1;
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
    if (input == NULL || words == NULL || word_count == NULL) {
        return 0;
    }
    
    char buffer[MAX_INPUT_LEN];
    if (!safe_strncpy(buffer, input, sizeof(buffer))) {
        return 0;
    }
    
    int count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && count < MAX_WORDS) {
        size_t token_len = safe_strlen(token, MAX_WORD_LEN);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            char clean_word[MAX_WORD_LEN];
            size_t clean_idx = 0;
            
            for (size_t i = 0; i < token_len && clean_idx < MAX_WORD_LEN - 1; i++) {
                if (isalpha((unsigned char)token[i])) {
                    clean_word[clean_idx++] = token[i];
                }
            }
            clean_word[clean_idx] = '\0';
            
            if (clean_idx > 0) {
                normalize_word(clean_word);
                
                int existing_idx = find_word_index(words, *word_count, clean_word);
                if (existing_idx >= 0) {
                    if (words[existing_idx].count < INT_MAX) {
                        words[existing_idx].count++;
                    }
                } else {
                    if (*word_count < MAX_WORDS) {
                        if (safe_strncpy(words[*word_count].word, clean_word, MAX_WORD_LEN)) {
                            words[*word_count].count = 1;
                            (*word_count)++;
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " \t\n");
        count++;
    }
    
    return 1;
}

void print_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Input contains invalid characters\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("\nWord frequency analysis:\n");