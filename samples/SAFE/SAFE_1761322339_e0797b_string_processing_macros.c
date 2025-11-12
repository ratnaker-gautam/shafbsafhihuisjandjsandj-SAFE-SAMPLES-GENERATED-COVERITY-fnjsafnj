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

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    int len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    *word_count = 0;
    int pos = 0;
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        char current_word[MAX_WORD_LEN] = {0};
        int word_pos = 0;
        
        while (pos < len && word_pos < MAX_WORD_LEN - 1 && !IS_WHITESPACE(input[pos])) {
            char c = input[pos];
            if (is_valid_char(c)) {
                current_word[word_pos] = TO_LOWER(c);
                word_pos++;
            }
            pos++;
        }
        
        if (word_pos > 0) {
            current_word[word_pos] = '\0';
            
            int found = 0;
            for (int i = 0; i < *word_count; i++) {
                if (strcmp(words[i].word, current_word) == 0) {
                    if (words[i].count < INT_MAX) {
                        words[i].count++;
                    }
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strlen(current_word, MAX_WORD_LEN) < MAX_WORD_LEN) {
                    strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
    
    return 0;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1] = {0};
    WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strnlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
    } else {
        print_word_counts(words, word_count);
    }
    
    return EXIT_SUCCESS;
}