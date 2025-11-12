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

void process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    int len = strlen(buffer);
    if (len >= MAX_INPUT_LEN) return;
    
    int i = 0;
    while (i < len) {
        while (i < len && IS_WHITESPACE(buffer[i])) i++;
        if (i >= len) break;
        
        int start = i;
        while (i < len && !IS_WHITESPACE(buffer[i])) i++;
        
        char temp_word[MAX_WORD_LEN];
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) continue;
        
        for (int j = 0; j < word_len; j++) {
            temp_word[j] = TO_LOWER(buffer[start + j]);
        }
        temp_word[word_len] = '\0';
        
        if (temp_word[0] == '\0') continue;
        
        int existing_index = find_word_index(words, *word_count, temp_word);
        if (existing_index != -1) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            if (*word_count < MAX_WORDS) {
                if (safe_strncpy(words[*word_count].word, temp_word, sizeof(words[*word_count].word)) == 0) {
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        printf("\nWord frequencies:\n");
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found\n");
    }
    
    return EXIT_SUCCESS;
}