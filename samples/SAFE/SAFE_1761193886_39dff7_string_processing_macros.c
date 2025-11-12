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
    char buffer[MAX_INPUT_LEN + 1];
    int word_count = 0;
    
    if (!is_valid_input(input)) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    to_lowercase(buffer);
    
    int i = 0;
    while (buffer[i] != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(buffer[i]) && buffer[i] != '\0') {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        int start = i;
        while (!IS_WHITESPACE(buffer[i]) && !IS_TERMINATOR(buffer[i]) && buffer[i] != '\0') {
            i++;
        }
        
        int end = i;
        int word_len = end - start;
        
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            strncpy(temp_word, &buffer[start], word_len);
            temp_word[word_len] = '\0';
            
            int existing_index = find_word(words, word_count, temp_word);
            if (existing_index != -1) {
                words[existing_index].count++;
            } else {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        if (IS_TERMINATOR(buffer[i])) {
            i++;
        }
    }
    
    return word_count;
}

void print_word_counts(WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = safe_strlen(input, sizeof(input));
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    int word_count = process_input(input, words);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return EXIT_SUCCESS;
    }
    
    print_word_counts(words, word_count);
    
    return EXIT_SUCCESS;
}