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

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int safe_tolower(int c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    int count = 0;
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) return -1;
    
    size_t pos = 0;
    while (pos < input_len && count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) pos++;
        if (pos >= input_len) break;
        
        size_t start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && !IS_TERMINATOR(input[pos])) pos++;
        
        size_t word_len = pos - start;
        if (word_len >= MAX_WORD_LEN - 1) word_len = MAX_WORD_LEN - 2;
        
        char temp_word[MAX_WORD_LEN];
        for (size_t i = 0; i < word_len; i++) {
            temp_word[i] = safe_tolower(input[start + i]);
        }
        temp_word[word_len] = '\0';
        
        if (word_len > 0) {
            int existing_index = find_word_index(words, count, temp_word);
            if (existing_index >= 0) {
                if (words[existing_index].count < INT_MAX) {
                    words[existing_index].count++;
                }
            } else {
                if (safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN) == 0) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
        
        if (pos < input_len && IS_TERMINATOR(input[pos])) pos++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    return EXIT_SUCCESS;
}