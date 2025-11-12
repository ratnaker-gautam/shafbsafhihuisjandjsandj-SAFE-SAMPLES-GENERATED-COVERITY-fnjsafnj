//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
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

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, max_len);
    if (len == 0 || len >= max_len) return 0;
    return 1;
}

int extract_words(const char *input, WordCount words[], int max_words) {
    if (!is_valid_input(input, MAX_INPUT_LEN) || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            if (word_len < MAX_WORD_LEN - 1) {
                words[word_count].word[word_len] = TO_LOWER(input[i]);
                word_len++;
            }
            i++;
        }
        
        if (word_len > 0) {
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    return word_count;
}

int count_unique_words(WordCount words[], int total_words) {
    if (words == NULL || total_words <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < total_words; i++) {
        if (words[i].count == 0) continue;
        
        int is_unique = 1;
        for (int j = i + 1; j < total_words; j++) {
            if (words[j].count > 0 && strcmp(words[i].word, words[j].word) == 0) {
                words[i].count += words[j].count;
                words[j].count = 0;
                is_unique = 0;
            }
        }
        
        if (is_unique) {
            unique_count++;
        }
    }
    
    return unique_count;
}

void print_word_stats(WordCount words[], int total_words) {
    if (words == NULL || total_words <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < total_words; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int total_words = extract_words(input, words, MAX_WORDS);
    if (total_words == 0) {
        printf("No words found in input.\n");
        return EXIT_SUCCESS;
    }
    
    int unique_words = count_unique_words(words, total_words);
    
    printf("\nTotal words: %d\n", total_words);
    printf("Unique words: %d\n\n", unique_words);
    
    print_word_stats(words, total_words);
    
    return EXIT_SUCCESS;
}