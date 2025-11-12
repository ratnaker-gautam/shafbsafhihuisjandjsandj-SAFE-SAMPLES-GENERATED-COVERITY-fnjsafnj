//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

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
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int add_word(WordCount *words, int *word_count, const char *word) {
    if (*word_count >= MAX_WORDS) return 0;
    size_t word_len = safe_strlen(word, MAX_WORD_LEN);
    if (word_len == 0 || word_len >= MAX_WORD_LEN) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 1;
        }
    }
    
    strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    to_lower_case(buffer);
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; buffer[i] != '\0'; i++) {
        if (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i])) {
            if (in_word) {
                current_word[word_pos] = '\0';
                if (!add_word(words, word_count, current_word)) {
                    return 0;
                }
                word_pos = 0;
                in_word = 0;
            }
        } else if (isalpha((unsigned char)buffer[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = buffer[i];
                in_word = 1;
            } else {
                return 0;
            }
        }
    }
    
    if (in_word) {
        current_word[word_pos] = '\0';
        if (!add_word(words, word_count, current_word)) {
            return 0;
        }
    }
    
    return 1;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
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
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}