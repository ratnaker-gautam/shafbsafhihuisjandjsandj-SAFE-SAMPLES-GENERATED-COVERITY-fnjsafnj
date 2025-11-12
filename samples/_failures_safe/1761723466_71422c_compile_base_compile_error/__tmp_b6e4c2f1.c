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
        while (i < text_len && !IS_WHITESPACE(text[i]) && !IS_TERMINATOR(text[i])) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN - 1) {
            char temp_word[MAX_WORD_LEN];
            size_t j;
            for (j = 0; j < word_len && j < MAX_WORD_LEN - 1; j++) {
                temp_word[j] = TO_LOWER(text[start + j]);
            }
            temp_word[j] = '\0';
            
            int found = 0;
            for (int k = 0; k < word_count; k++) {
                if (strcmp(words[k].word, temp_word) == 0) {
                    words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && word_count < max_words) {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        while (i < text_len && (IS_WHITESPACE(text[i]) || IS_TERMINATOR(text[i]))) i++;
    }
    
    return word_count;
}

void sort_words(WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!validate_input(input, MAX_INPUT_LEN)) {
        printf("Invalid input characters detected.\n");
        return 1;
    }
    
    if (input_len == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord