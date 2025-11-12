//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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
    char *copy = malloc(len + 1);
    if (copy != NULL) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

char *to_lowercase(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *lower = malloc(len + 1);
    if (lower == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower((unsigned char)s[i]);
    }
    lower[len] = '\0';
    return lower;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    struct WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
    char *input_copy = strdup_safe(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        int start = 0, end = token_len - 1;
        while (start < (int)token_len && !is_word_char(token[start])) start++;
        while (end >= 0 && !is_word_char(token[end])) end--;
        
        if (start <= end) {
            char clean_word[256];
            int clean_len = end - start + 1;
            if (clean_len >= (int)sizeof(clean_word)) {
                clean_len = sizeof(clean_word) - 1;
            }
            memcpy(clean_word, token + start, clean_len);
            clean_word[clean_len] = '\0';
            
            char *lower_word = to_lowercase(clean_word);
            if (lower_word == NULL) {
                free(input_copy);
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            
            int index = find_word_index(words_stack, word_count, lower_word);
            if (index >= 0) {
                words_stack[index].count++;
                free(lower_word);
            } else {
                char *word_heap = strdup_safe(lower_word);
                free(lower_word);
                if (word_heap == NULL) {
                    free(input_copy);
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                words_stack[word_count].word = word_heap;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    free_words(words_stack, word_count);
    
    return 0;
}