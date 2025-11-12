//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define ALPHABET_SIZE 26
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN + 1];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    size_t i = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) i++;
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) i++;
        
        size_t word_len = i - start;
        if (word_len == 0) continue;
        if (word_len > MAX_WORD_LEN) return 0;
        
        char temp_word[MAX_WORD_LEN + 1];
        for (size_t j = 0; j < word_len; j++) {
            temp_word[j] = TO_LOWER(input[start + j]);
        }
        temp_word[word_len] = '\0';
        
        int found = 0;
        for (int k = 0; k < count; k++) {
            if (strcmp(words[k].word, temp_word) == 0) {
                if (words[k].count < INT_MAX) {
                    words[k].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (safe_strlen(temp_word, MAX_WORD_LEN) != word_len) return 0;
            strncpy(words[count].word, temp_word, MAX_WORD_LEN);
            words[count].word[MAX_WORD_LEN] = '\0';
            words[count].count = 1;
            count++;
        }
    }
    
    *word_count = count;
    return 1;
}

void sort_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}