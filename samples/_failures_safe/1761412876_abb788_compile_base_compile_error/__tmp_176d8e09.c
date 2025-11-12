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
        word[i] = TO_LOWER(word[i]);
    }
    
    size_t write_pos = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i]) || word[i] == '\'') {
            word[write_pos++] = word[i];
        }
    }
    word[write_pos] = '\0';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) {
        return 0;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= sizeof(buffer)) return 0;
    
    memcpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    
    *word_count = 0;
    char *token = buffer;
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (IS_WHITESPACE(buffer[i]) || IS_TERMINATOR(buffer[i]) || buffer[i] == '\0') {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    char current_word[MAX_WORD_LEN];
                    if (word_len >= sizeof(current_word)) continue;
                    
                    memcpy(current_word, &buffer[word_start], word_len);
                    current_word[word_len] = '\0';
                    normalize_word(current_word);
                    
                    if (current_word[0] != '\0') {
                        int found = 0;
                        for (int j = 0; j < *word_count; j++) {
                            if (strcmp(words[j].word, current_word) == 0) {
                                if (words[j].count < INT_MAX) {
                                    words[j].count++;
                                }
                                found = 1;
                                break;
                            }
                        }
                        
                        if (!found) {
                            size_t copy_len = safe_strlen(current_word, MAX_WORD_LEN - 1);
                            if (copy_len < sizeof(words[*word_count].word)) {
                                memcpy(words[*word_count].word, current_word, copy_len);
                                words[*word_count].word[copy_len] = '\0';
                                words[*word_count].count = 1;
                                (*word_count)++;
                            }
                        }
                    }
                }
                in_word = 0;
            }
        } else if (!in_word) {
            word_start = i;
            in_word = 1;
        }
    }
    
    return 1;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "