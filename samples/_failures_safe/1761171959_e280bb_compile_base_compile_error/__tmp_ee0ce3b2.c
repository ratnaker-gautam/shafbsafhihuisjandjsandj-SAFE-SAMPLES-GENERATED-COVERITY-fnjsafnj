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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void process_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    
    while (len > 0 && !isalnum((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
    
    size_t start = 0;
    while (start < len && !isalnum((unsigned char)word[start])) {
        start++;
    }
    
    if (start > 0) {
        memmove(word, word + start, len - start + 1);
    }
}

int tokenize_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    *word_count = 0;
    char *ptr = buffer;
    
    while (*ptr != '\0' && *word_count < MAX_WORDS) {
        while (IS_WHITESPACE(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        char *word_start = ptr;
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr)) ptr++;
        
        char temp_word[MAX_WORD_LEN];
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        if (safe_strncpy(temp_word, word_start, word_len + 1) != 0) continue;
        process_word(temp_word);
        
        if (strlen(temp_word) == 0) continue;
        
        int existing_index = find_word(words, *word_count, temp_word);
        if (existing_index >= 0) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            if (safe_strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
        
        if (*ptr != '\0') ptr++;
    }
    
    return 0;
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
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (tokenize_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EX