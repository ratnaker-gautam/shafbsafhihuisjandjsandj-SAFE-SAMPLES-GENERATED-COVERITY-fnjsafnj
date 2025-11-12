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

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *dup = malloc(len + 1);
    if (dup != NULL) {
        memcpy(dup, s, len + 1);
    }
    return dup;
}

char *to_lowercase(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *lower = malloc(len + 1);
    if (lower == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower(s[i]);
    }
    lower[len] = '\0';
    return lower;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'';
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(struct WordCount *words, int count) {
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct WordCount *words = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *input_copy = strdup_safe(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(words);
        return 1;
    }
    
    char *token = strtok(input_copy, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        char *clean_token = malloc(strlen(token) + 1);
        if (clean_token == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(input_copy);
            free_words(words, word_count);
            free(words);
            return 1;
        }
        
        int j = 0;
        for (size_t i = 0; i < strlen(token); i++) {
            if (is_word_char(token[i])) {
                clean_token[j++] = token[i];
            }
        }
        clean_token[j] = '\0';
        
        if (strlen(clean_token) > 0) {
            char *lower_word = to_lowercase(clean_token);
            if (lower_word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(clean_token);
                free(input_copy);
                free_words(words, word_count);
                free(words);
                return 1;
            }
            
            int index = find_word_index(words, word_count, lower_word);
            if (index >= 0) {
                words[index].count++;
            } else {
                words[word_count].word = strdup_safe(lower_word);
                if (words[word_count].word == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(lower_word);
                    free(clean_token);
                    free(input_copy);
                    free_words(words, word_count);
                    free(words);
                    return 1;
                }
                words[word_count].count = 1;
                word_count++;
            }
            free(lower_word);
        }
        free(clean_token);
        token = strtok(NULL, " \t");
    }
    
    free(input_copy);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    free_words(words, word_count);
    free(words);
    
    return 0;
}