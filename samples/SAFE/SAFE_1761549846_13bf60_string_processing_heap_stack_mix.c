//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *to_lower_case(const char *str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

WordCount *find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return &words[i];
        }
    }
    return NULL;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (!fgets(input, sizeof(input), stdin)) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (!word_counts) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *input_copy = malloc(input_len + 1);
    if (!input_copy) {
        fprintf(stderr, "Memory allocation failed\n");
        free(word_counts);
        return 1;
    }
    strcpy(input_copy, input);
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        char *clean_token = malloc(token_len + 1);
        if (!clean_token) {
            fprintf(stderr, "Memory allocation failed\n");
            free(input_copy);
            free(word_counts);
            return 1;
        }
        
        int clean_len = 0;
        for (size_t i = 0; i < token_len; i++) {
            if (is_valid_char(token[i])) {
                clean_token[clean_len++] = token[i];
            }
        }
        clean_token[clean_len] = '\0';
        
        if (clean_len > 0) {
            char *lower_token = to_lower_case(clean_token);
            if (!lower_token) {
                fprintf(stderr, "Memory allocation failed\n");
                free(clean_token);
                free(input_copy);
                free(word_counts);
                return 1;
            }
            
            WordCount *existing = find_word(word_counts, word_count, lower_token);
            if (existing) {
                existing->count++;
                free(lower_token);
            } else {
                word_counts[word_count].word = lower_token;
                word_counts[word_count].count = 1;
                word_count++;
            }
        }
        
        free(clean_token);
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        free(word_counts);
        return 1;
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}