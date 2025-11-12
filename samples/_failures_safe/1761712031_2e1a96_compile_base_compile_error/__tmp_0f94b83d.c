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

int is_valid_char(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '\'' || c == '-';
}

int normalize_word(const char *src, char *dest, size_t dest_size) {
    if (src == NULL || dest == NULL || dest_size == 0) {
        return 0;
    }
    
    size_t i = 0;
    size_t j = 0;
    while (src[i] != '\0' && j < dest_size - 1) {
        if (is_valid_char(src[i])) {
            dest[j] = TO_LOWER(src[i]);
            j++;
        }
        i++;
    }
    dest[j] = '\0';
    
    return j > 0;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char temp_word[MAX_WORD_LEN];
            size_t copy_len = word_len;
            if (copy_len >= MAX_WORD_LEN) {
                copy_len = MAX_WORD_LEN - 1;
            }
            
            for (size_t j = 0; j < copy_len; j++) {
                temp_word[j] = input[start + j];
            }
            temp_word[copy_len] = '\0';
            
            char normalized[MAX_WORD_LEN];
            if (normalize_word(temp_word, normalized, sizeof(normalized))) {
                int existing_index = find_word_index(words, word_count, normalized);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    size_t norm_len = safe_strlen(normalized, MAX_WORD_LEN);
                    if (norm_len > 0 && norm_len < MAX_WORD_LEN) {
                        strncpy(words[word_count].word, normalized, MAX_WORD_LEN - 1);
                        words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies:\n");
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
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len == 0 || (input_len == MAX_INPUT_LEN - 1 && input[MAX_INPUT_LEN - 2] != '\n')) {
        fprintf(stderr, "Input too long or invalid\n");
        return EXIT_FAILURE;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    int word_count = process_input(input, words, MAX_WORDS);
    
    if (word_count