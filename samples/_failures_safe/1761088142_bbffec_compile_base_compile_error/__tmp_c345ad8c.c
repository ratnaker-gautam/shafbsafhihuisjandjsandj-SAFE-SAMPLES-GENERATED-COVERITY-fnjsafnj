//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define TO_UPPER(c) (((c) >= 'a' && (c) <= 'z') ? ((c) - 'a' + 'A') : (c))
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) - 'A' + 'a') : (c))

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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    int len = safe_strlen(str, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    for (int i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i]) && !IS_WHITESPACE(str[i])) {
            return 0;
        }
    }
    return 1;
}

int extract_words(const char *input, WordCount words[], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    int count = 0;
    int pos = 0;
    int input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (pos < input_len && count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        int word_start = pos;
        int word_len = 0;
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN) {
            pos++;
            word_len++;
        }
        
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            for (int i = 0; i < word_len; i++) {
                char c = input[word_start + i];
                words[count].word[i] = TO_LOWER(c);
            }
            words[count].word[word_len] = '\0';
            words[count].count = 1;
            count++;
        }
    }
    
    *word_count = count;
    return 1;
}

int process_word_counts(WordCount words[], int word_count) {
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

void print_results(const WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        char display_word[MAX_WORD_LEN + 1];
        int len = safe_strlen(words[i].word, MAX_WORD_LEN);
        
        for (int j = 0; j < len; j++) {
            if (j == 0) {
                display_word[j] = TO_UPPER(words[i].word[j]);
            } else {
                display_word[j] = words[i].word[j];
            }
        }
        display_word[len] = '\0';
        
        printf("%-20s: %d\n", display_word, words[i].count);
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
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }