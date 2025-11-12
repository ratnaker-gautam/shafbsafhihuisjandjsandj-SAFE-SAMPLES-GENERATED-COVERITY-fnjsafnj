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
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

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
    return (c >= 32 && c <= 126) || c == '\t' || c == '\n';
}

int validate_input(const char *input, size_t max_len) {
    if (input == NULL) return 0;
    for (size_t i = 0; i < max_len && input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *text, WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t text_len = safe_strlen(text, MAX_INPUT_LEN);
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && IS_WHITESPACE(text[i])) i++;
        
        if (i >= text_len) break;
        
        size_t start = i;
        while (i < text_len && !IS_WHITESPACE(text[i]) && !IS_TERMINATOR(text[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            strncpy(words[word_count].word, &text[start], word_len);
            words[word_count].word[word_len] = '\0';
            
            for (size_t j = 0; j < word_len; j++) {
                words[word_count].word[j] = TO_LOWER(words[word_count].word[j]);
            }
            
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }
    
    return word_count;
}

void sort_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    if (!validate_input(input, sizeof(input))) {
        fprintf(stderr, "Invalid characters in input\n");
        return 1;
    }
    
    int initial_word_count = extract_words(input, words, MAX_WORDS);
    if (initial_word_count <= 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_word_count = count_unique_words(words, initial_word_count);
    if (unique_word_count <= 0) {
        printf("Error