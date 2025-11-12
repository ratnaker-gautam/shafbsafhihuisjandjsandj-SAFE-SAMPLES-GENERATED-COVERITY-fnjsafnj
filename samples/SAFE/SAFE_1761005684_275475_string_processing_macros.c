//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
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

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    if (*word_count < 0 || *word_count > MAX_WORDS) return -1;
    
    int current_word_len = 0;
    char current_word[MAX_WORD_LEN];
    int in_word = 0;
    
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (IS_WHITESPACE(input[i])) {
            if (in_word && current_word_len > 0) {
                current_word[current_word_len] = '\0';
                
                if (*word_count >= MAX_WORDS) {
                    return -1;
                }
                
                int existing_index = find_word_index(words, *word_count, current_word);
                if (existing_index >= 0) {
                    if (words[existing_index].count < INT_MAX) {
                        words[existing_index].count++;
                    }
                } else {
                    if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                
                current_word_len = 0;
                in_word = 0;
            }
        } else if (isalpha((unsigned char)input[i])) {
            if (current_word_len < MAX_WORD_LEN - 1) {
                current_word[current_word_len++] = TO_LOWER((unsigned char)input[i]);
                in_word = 1;
            }
        }
    }
    
    if (in_word && current_word_len > 0) {
        current_word[current_word_len] = '\0';
        
        if (*word_count >= MAX_WORDS) {
            return -1;
        }
        
        int existing_index = find_word_index(words, *word_count, current_word);
        if (existing_index >= 0) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            if (safe_strncpy(words[*word_count].word, current_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    
    return 0;
}

void print_word_counts(const WordCount *words, int count) {
    if (words == NULL || count <= 0 || count > MAX_WORDS) return;
    
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return EXIT_SUCCESS;
    }
    
    printf("\nWord frequency analysis:\n");
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}