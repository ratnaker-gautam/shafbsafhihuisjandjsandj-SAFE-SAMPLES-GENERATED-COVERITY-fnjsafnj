//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = safe_strlen(str, MAX_INPUT_LEN + 1);
    return len > 0 && len <= MAX_INPUT_LEN;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words) {
    if (!is_valid_input(input) || words == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN + 1];
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len >= sizeof(buffer)) return 0;
    
    memcpy(buffer, input, len);
    buffer[len] = '\0';
    
    int word_count = 0;
    int pos = 0;
    
    while (pos < len && word_count < MAX_WORDS) {
        while (pos < len && IS_WHITESPACE(buffer[pos])) {
            pos++;
        }
        
        if (pos >= len) break;
        
        int start = pos;
        while (pos < len && !IS_WHITESPACE(buffer[pos]) && !IS_TERMINATOR(buffer[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            memcpy(temp_word, &buffer[start], word_len);
            temp_word[word_len] = '\0';
            to_lowercase(temp_word);
            
            int existing_index = find_word(words, word_count, temp_word);
            if (existing_index >= 0) {
                if (words[existing_index].count < INT_MAX) {
                    words[existing_index].count++;
                }
            } else {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        if (pos < len && IS_TERMINATOR(buffer[pos])) {
            pos++;
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
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
    
    memset(words, 0, sizeof(words));
    int word_count = process_input(input, words);
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}