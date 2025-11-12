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
    return isalpha(c) || c == '\'';
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

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words_stack[MAX_WORDS];
    WordCount *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int valid_word = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!is_word_char(token[i])) {
                    valid_word = 0;
                    break;
                }
            }
            
            if (valid_word) {
                int existing_index = find_word(words_stack, word_count, token);
                if (existing_index >= 0) {
                    words_stack[existing_index].count++;
                } else {
                    char *word_copy = malloc(token_len + 1);
                    if (word_copy == NULL) {
                        free(lower_input);
                        free_words(words_stack, word_count);
                        fprintf(stderr, "Memory allocation failed\n");
                        return 1;
                    }
                    strcpy(word_copy, token);
                    words_stack[word_count].word = word_copy;
                    words_stack[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " ");
    }
    
    if (word_count > 0) {
        words_heap = malloc(sizeof(WordCount) * word_count);
        if (words_heap == NULL) {
            free(lower_input);
            free_words(words_stack, word_count);
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < word_count; i++) {
            words_heap[i] = words_stack[i];
        }
        
        qsort(words_heap, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
        }
        
        free(words_heap);
    } else {
        printf("No valid words found in input.\n");
    }
    
    free(lower_input);
    free_words(words_stack, word_count);
    
    return 0;
}