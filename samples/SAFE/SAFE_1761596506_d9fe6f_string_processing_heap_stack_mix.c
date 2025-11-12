//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_INPUT_LEN) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

char **split_into_words(const char *text, int *word_count) {
    if (text == NULL || word_count == NULL) return NULL;
    
    size_t len = strlen(text);
    if (len == 0 || len >= MAX_INPUT_LEN) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    char *copy = malloc(len + 1);
    if (copy == NULL) {
        free(words);
        return NULL;
    }
    strcpy(copy, text);
    
    *word_count = 0;
    char *token = strtok(copy, " \t\n\r");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[*word_count] = malloc(token_len + 1);
            if (words[*word_count] != NULL) {
                strcpy(words[*word_count], token);
                (*word_count)++;
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(copy);
    return words;
}

int find_word_index(struct WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(char **words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

void free_word_counts(struct WordCount *word_counts, int count) {
    if (word_counts == NULL) return;
    for (int i = 0; i < count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count;
    char **words = split_into_words(input, &word_count);
    if (words == NULL || word_count == 0) {
        printf("No valid words found\n");
        if (words != NULL) free_words(words, word_count);
        return 1;
    }
    
    struct WordCount *word_counts = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (word_counts == NULL) {
        printf("Memory allocation failed\n");
        free_words(words, word_count);
        return 1;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char *lower_word = to_lower_case(words[i]);
        if (lower_word == NULL) continue;
        
        int found_index = find_word_index(word_counts, unique_count, lower_word);
        
        if (found_index == -1) {
            if (unique_count < MAX_WORDS) {
                word_counts[unique_count].word = lower_word;
                word_counts[unique_count].count = 1;
                unique_count++;
            } else {
                free(lower_word);
            }
        } else {
            word_counts[found_index].count++;
            free(lower_word);
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free_words(words, word_count);
    free_word_counts(word_counts, unique_count);
    
    return 0;
}