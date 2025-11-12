//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
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

int is_valid_char(char c) {
    return (c >= 32 && c <= 126) || c == '\t' || c == '\n';
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    for (size_t i = 0; i < max_len && input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, WordCount words[], int max_words) {
    int word_count = 0;
    size_t i = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            i++;
            word_len++;
        }
        
        if (word_len > 0) {
            for (size_t j = 0; j < word_len; j++) {
                char c = TO_LOWER(input[word_start + j]);
                if (isalpha(c)) {
                    words[word_count].word[j] = c;
                } else {
                    words[word_count].word[j] = ' ';
                }
            }
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void count_word_frequency(WordCount words[], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) continue;
            
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
}

void print_word_frequencies(WordCount words[], int word_count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0 && safe_strlen(words[i].word, MAX_WORD_LEN) > 0) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    if (!validate_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequency(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}