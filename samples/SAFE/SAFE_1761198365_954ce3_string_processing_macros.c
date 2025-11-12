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
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int find_word_index(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL || count < 0) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void normalize_word(char *word) {
    if (word == NULL || word[0] == '\0') return;
    
    size_t len = strlen(word);
    size_t write_pos = 0;
    
    for (size_t i = 0; i < len; i++) {
        char c = word[i];
        if (isalpha((unsigned char)c)) {
            word[write_pos++] = TO_LOWER(c);
        }
    }
    word[write_pos] = '\0';
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(buffer);
    size_t pos = 0;
    
    while (pos < input_len && word_count < max_words) {
        while (pos < input_len && IS_WHITESPACE(buffer[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        size_t word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(buffer[pos])) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, &buffer[word_start], word_len + 1) == 0) {
                normalize_word(temp_word);
                
                if (temp_word[0] != '\0') {
                    int existing_index = find_word_index(words, word_count, temp_word);
                    if (existing_index >= 0) {
                        if (words[existing_index].count < INT_MAX) {
                            words[existing_index].count++;
                        }
                    } else {
                        if (safe_strncpy(words[word_count].word, temp_word, MAX_WORD_LEN) == 0) {
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
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
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (input[0] == '\0') {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count > 0) {
        printf("\nWord frequency analysis:\n");
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found in input\n");
    }
    
    return 0;
}