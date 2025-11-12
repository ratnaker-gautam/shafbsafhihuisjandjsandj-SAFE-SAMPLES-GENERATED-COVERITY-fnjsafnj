//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
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
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    for (size_t i = 0; i < len; i++) {
        if (IS_TERMINATOR(word[i])) {
            word[i] = '\0';
            break;
        }
        word[i] = TO_LOWER(word[i]);
    }
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    if (input_len >= sizeof(buffer)) return 0;
    memcpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    
    char *token = buffer;
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (IS_WHITESPACE(buffer[i]) || buffer[i] == '\0' || IS_TERMINATOR(buffer[i])) {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    memcpy(words[*word_count].word, &buffer[word_start], word_len);
                    words[*word_count].word[word_len] = '\0';
                    normalize_word(words[*word_count].word);
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                in_word = 0;
            }
        } else {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        }
    }
    
    return 1;
}

void count_word_frequency(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i].word, words[j].word) == 0) {
                    words[i].count++;
                    words[j].count = 0;
                }
            }
        }
    }
}

void print_word_frequency(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0 && words[i].word[0] != '\0') {
            printf("%-15s: %d\n", words[i].word, words[i].count);
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
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return