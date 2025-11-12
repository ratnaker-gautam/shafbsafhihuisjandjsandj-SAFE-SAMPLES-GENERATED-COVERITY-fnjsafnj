//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
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

void safe_strncpy(char *dest, const char *src, size_t n) {
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
}

int safe_strcmp(const char *s1, const char *s2, size_t max_len) {
    size_t i = 0;
    while (i < max_len && s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
        i++;
    }
    if (i < max_len) {
        return s1[i] - s2[i];
    }
    return 0;
}

void normalize_word(char *word) {
    size_t len = safe_strlen(word, MAX_WORD_LEN);
    size_t i, j = 0;
    char temp[MAX_WORD_LEN + 1];
    
    for (i = 0; i < len && j < MAX_WORD_LEN; i++) {
        if (isalnum((unsigned char)word[i])) {
            temp[j++] = TO_LOWER(word[i]);
        }
    }
    temp[j] = '\0';
    safe_strncpy(word, temp, MAX_WORD_LEN + 1);
}

int process_input(const char *input, WordCount *words, int *word_count) {
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    char buffer[MAX_INPUT_LEN + 1];
    char current_word[MAX_WORD_LEN + 1];
    size_t i = 0, word_len = 0;
    int count = 0;
    
    safe_strncpy(buffer, input, MAX_INPUT_LEN + 1);
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(buffer[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        word_len = 0;
        while (i < input_len && word_len < MAX_WORD_LEN && !IS_WHITESPACE(buffer[i])) {
            current_word[word_len++] = buffer[i++];
        }
        current_word[word_len] = '\0';
        
        if (word_len > 0) {
            normalize_word(current_word);
            
            if (safe_strlen(current_word, MAX_WORD_LEN) > 0) {
                int found = 0;
                for (int j = 0; j < count; j++) {
                    if (safe_strcmp(words[j].word, current_word, MAX_WORD_LEN) == 0) {
                        if (words[j].count < INT_MAX) {
                            words[j].count++;
                        }
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    safe_strncpy(words[count].word, current_word, MAX_WORD_LEN + 1);
                    words[count].count = 1;
                    count++;
                }
            }
        }
    }
    
    *word_count = count;
    return 1;
}

void print_results(const WordCount *words, int word_count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (safe_strlen(input, MAX_INPUT_LEN) ==