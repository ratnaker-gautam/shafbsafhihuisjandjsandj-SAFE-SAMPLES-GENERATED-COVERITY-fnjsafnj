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

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return 0;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 1;
}

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) return 0;
    
    size_t i = 0;
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) i++;
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && is_alpha_char(input[i])) i++;
        size_t end = i;
        
        if (end > start) {
            size_t word_len = end - start;
            if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
            
            char temp_word[MAX_WORD_LEN];
            for (size_t j = 0; j < word_len; j++) {
                temp_word[j] = TO_LOWER(input[start + j]);
            }
            temp_word[word_len] = '\0';
            
            int found = 0;
            for (int k = 0; k < count; k++) {
                if (strcmp(words[k].word, temp_word) == 0) {
                    words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN)) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
        
        while (i < input_len && !is_alpha_char(input[i]) && !IS_WHITESPACE(input[i])) i++;
    }
    
    *word_count = count;
    return 1;
}

void print_word_counts(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}