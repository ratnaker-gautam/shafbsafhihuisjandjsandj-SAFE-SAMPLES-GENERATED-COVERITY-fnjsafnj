//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len >= sizeof(buffer)) return 0;
    
    strncpy(buffer, input, input_len);
    buffer[input_len] = '\0';
    to_lowercase(buffer);
    
    *word_count = 0;
    size_t pos = 0;
    
    while (pos < input_len && *word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(buffer[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        size_t word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(buffer[pos]) && !IS_TERMINATOR(buffer[pos])) {
            pos++;
        }
        
        size_t word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[*word_count].word, &buffer[word_start], word_len);
            words[*word_count].word[word_len] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
        
        if (pos < input_len && IS_TERMINATOR(buffer[pos])) {
            pos++;
        }
    }
    
    return 1;
}

int count_word_frequency(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
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
    
    int valid_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            words[valid_count] = words[i];
            valid_count++;
        }
    }
    
    return valid_count;
}

void print_word_frequencies(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
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
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
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
    
    int unique_count = count_word_frequency(words, word_count);
    
    if (unique_count == 0) {
        printf("No valid words to count\n");
        return 1;
    }
    
    print_word_frequencies(words, unique_count);
    return