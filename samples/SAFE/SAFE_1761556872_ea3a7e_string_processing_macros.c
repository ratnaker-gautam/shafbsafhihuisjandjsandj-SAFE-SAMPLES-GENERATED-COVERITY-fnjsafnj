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

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words, int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    int word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && word_count < max_words) {
        while (IS_WHITESPACE(buffer[i]) && buffer[i] != '\0') i++;
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (!IS_WHITESPACE(buffer[i]) && !IS_TERMINATOR(buffer[i]) && buffer[i] != '\0') i++;
        
        size_t len = i - start;
        if (len > 0 && len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, &buffer[start], len + 1) != 0) continue;
            to_lower_case(temp_word);
            
            int found_index = find_word(words, word_count, temp_word);
            if (found_index >= 0) {
                if (words[found_index].count < INT_MAX) {
                    words[found_index].count++;
                }
            } else {
                if (safe_strncpy(words[word_count].word, temp_word, sizeof(words[word_count].word)) == 0) {
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        while (IS_TERMINATOR(buffer[i]) && buffer[i] != '\0') i++;
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
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
    
    int word_count = process_input(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return EXIT_SUCCESS;
    }
    
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}