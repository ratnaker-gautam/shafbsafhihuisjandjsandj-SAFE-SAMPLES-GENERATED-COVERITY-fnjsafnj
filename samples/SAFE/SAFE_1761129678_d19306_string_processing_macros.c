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
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int split_into_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        char c = input[i];
        if (IS_WHITESPACE(c) || c == '\0') {
            if (in_word && pos > 0) {
                words[word_count][pos] = '\0';
                word_count++;
                pos = 0;
                in_word = 0;
            }
        } else {
            if (pos < MAX_WORD_LEN - 1) {
                words[word_count][pos++] = TO_LOWER(c);
            }
            in_word = 1;
        }
    }
    
    return word_count;
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
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(word_counts, *unique_count, words[i]);
        if (index >= 0) {
            if (word_counts[index].count < INT_MAX) {
                word_counts[index].count++;
            }
        } else if (*unique_count < MAX_WORDS) {
            strncpy(word_counts[*unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[*unique_count].count = 1;
            (*unique_count)++;
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
    int word_count, unique_count;
    
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
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequency(words, word_count, word_counts, &unique_count);
    print_word_frequencies(word_counts, unique_count);
    
    return 0;
}