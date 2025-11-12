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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char stack_buffer[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    strncpy(stack_buffer, input, sizeof(stack_buffer) - 1);
    stack_buffer[sizeof(stack_buffer) - 1] = '\0';
    
    char *lower_input = to_lowercase(stack_buffer);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(lower_input);
        return 1;
    }
    
    int word_count = 0;
    char *token = strtok(lower_input, " \t\n\r.,;:!?");
    
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            int found_idx = find_word(word_counts, word_count, token);
            if (found_idx >= 0) {
                word_counts[found_idx].count++;
            } else {
                char *word_copy = malloc(strlen(token) + 1);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free_words(word_counts, word_count);
                    free(word_counts);
                    free(lower_input);
                    return 1;
                }
                strcpy(word_copy, token);
                word_counts[word_count].word = word_copy;
                word_counts[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free_words(word_counts, word_count);
    free(word_counts);
    free(lower_input);
    
    return 0;
}