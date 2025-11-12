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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    size_t i = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            size_t j;
            for (j = 0; j < word_len; j++) {
                words[count][j] = tolower(input[word_start + j]);
            }
            words[count][word_len] = '\0';
            count++;
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    *word_count = count;
    return count > 0;
}

int find_word_index(WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, WordCount *word_counts, int *unique_count) {
    *unique_count = 0;
    
    for (int i = 0; i < word_count && *unique_count < MAX_WORDS; i++) {
        int idx = find_word_index(word_counts, *unique_count, words[i]);
        if (idx >= 0) {
            if (word_counts[idx].count < INT_MAX) {
                word_counts[idx].count++;
            }
        } else {
            size_t len = safe_strlen(words[i], MAX_WORD_LEN);
            if (len > 0) {
                strncpy(word_counts[*unique_count].word, words[i], MAX_WORD_LEN - 1);
                word_counts[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
                word_counts[*unique_count].count = 1;
                (*unique_count)++;
            }
        }
    }
}

void print_word_frequencies(WordCount *word_counts, int unique_count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!split_into_words(input, words, &word_count)) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    count_word_frequency(words, word_count, word_counts, &unique_count);
    print_word_frequencies(word_counts, unique_count);
    
    return 0;
}