//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'';
}

WordCount *find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return &words[i];
        }
    }
    return NULL;
}

void free_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
    char *word_start = lower_input;
    while (*word_start && word_count < MAX_WORDS) {
        while (*word_start && !is_word_char(*word_start)) {
            word_start++;
        }
        if (!*word_start) break;
        
        char *word_end = word_start;
        while (*word_end && is_word_char(*word_end)) {
            word_end++;
        }
        
        size_t word_len = word_end - word_start;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) {
                free_words(words_stack, word_count);
                free(lower_input);
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            memcpy(word, word_start, word_len);
            word[word_len] = '\0';
            
            WordCount *existing = find_word(words_stack, word_count, word);
            if (existing != NULL) {
                existing->count++;
                free(word);
            } else {
                words_stack[word_count].word = word;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
        word_start = word_end;
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    free_words(words_stack, word_count);
    free(lower_input);
    
    return 0;
}