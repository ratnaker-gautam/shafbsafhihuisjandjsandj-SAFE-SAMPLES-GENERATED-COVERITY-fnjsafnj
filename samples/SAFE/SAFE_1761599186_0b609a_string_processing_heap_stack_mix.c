//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

char* to_lowercase(const char* str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_WORD_LEN) return NULL;
    
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    
    return result;
}

int is_valid_word(const char* word) {
    if (word == NULL || *word == '\0') return 0;
    
    for (size_t i = 0; word[i] != '\0'; i++) {
        if (!isalpha((unsigned char)word[i])) {
            return 0;
        }
    }
    return 1;
}

int find_word_index(struct WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int main(void) {
    char input[1024];
    struct WordCount* word_counts = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    
    printf("Enter text (max 1023 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(word_counts);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    char* token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        char* lower_token = to_lowercase(token);
        if (lower_token == NULL) {
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        if (is_valid_word(lower_token)) {
            int index = find_word_index(word_counts, word_count, lower_token);
            if (index >= 0) {
                if (word_counts[index].count < INT_MAX) {
                    word_counts[index].count++;
                }
            } else {
                if (strlen(lower_token) < MAX_WORD_LEN) {
                    strncpy(word_counts[word_count].word, lower_token, MAX_WORD_LEN - 1);
                    word_counts[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    word_counts[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        free(lower_token);
        token = strtok(NULL, " \t\n\r");
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free(word_counts);
    return 0;
}