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
    if (len == 0) return NULL;
    
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

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    char *input_copy = malloc(input_len + 1);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    strcpy(input_copy, input);
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        if (token_len > 0) {
            char *clean_token = malloc(token_len + 1);
            if (clean_token == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(input_copy);
                free_words(words, word_count);
                return 1;
            }
            
            int clean_index = 0;
            for (size_t i = 0; i < token_len; i++) {
                if (is_word_char(token[i])) {
                    clean_token[clean_index++] = token[i];
                }
            }
            clean_token[clean_index] = '\0';
            
            if (clean_index > 0) {
                char *lower_word = to_lower_case(clean_token);
                if (lower_word == NULL) {
                    free(clean_token);
                    continue;
                }
                
                int existing_index = find_word_index(words, word_count, lower_word);
                if (existing_index >= 0) {
                    words[existing_index].count++;
                    free(lower_word);
                } else {
                    words[word_count].word = lower_word;
                    words[word_count].count = 1;
                    word_count++;
                }
            }
            free(clean_token);
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis (top %d words):\n", word_count > 10 ? 10 : word_count);
    int display_count = word_count > 10 ? 10 : word_count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    
    free_words(words, word_count);
    return 0;
}