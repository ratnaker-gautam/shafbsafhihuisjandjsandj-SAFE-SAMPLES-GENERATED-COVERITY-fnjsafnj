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

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
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
        return 1;
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
        char *clean_token = malloc(strlen(token) + 1);
        if (clean_token == NULL) {
            free(input_copy);
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        
        int clean_index = 0;
        for (int i = 0; token[i] != '\0'; i++) {
            if (is_word_char(token[i])) {
                clean_token[clean_index++] = token[i];
            }
        }
        clean_token[clean_index] = '\0';
        
        if (clean_index > 0) {
            char *lower_token = to_lowercase(clean_token);
            if (lower_token == NULL) {
                free(clean_token);
                free(input_copy);
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            
            int existing_index = find_word_index(words_stack, word_count, lower_token);
            if (existing_index >= 0) {
                words_stack[existing_index].count++;
                free(lower_token);
            } else {
                words_stack[word_count].word = strdup_safe(lower_token);
                if (words_stack[word_count].word == NULL) {
                    free(lower_token);
                    free(clean_token);
                    free(input_copy);
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                words_stack[word_count].count = 1;
                word_count++;
                free(lower_token);
            }
        }
        
        free(clean_token);
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    if (word_count > 0) {
        qsort(words_stack, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("Word frequencies (sorted by count):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
            free(words_stack[i].word);
        }
    } else {
        printf("No valid words found\n");
    }
    
    return 0;
}