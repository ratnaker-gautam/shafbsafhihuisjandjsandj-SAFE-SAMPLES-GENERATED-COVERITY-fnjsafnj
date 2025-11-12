//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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
    return isalnum(c) || c == '\'' || c == '-';
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
    char input[MAX_INPUT_LEN];
    
    printf("Enter text: ");
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
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    char *word_heap[MAX_WORDS];
    int heap_word_count = 0;
    
    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS && heap_word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char *clean_word = malloc(token_len + 1);
            if (clean_word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(lower_input);
                for (int i = 0; i < heap_word_count; i++) free(word_heap[i]);
                return 1;
            }
            
            int clean_len = 0;
            for (size_t i = 0; i < token_len; i++) {
                if (is_word_char(token[i])) {
                    clean_word[clean_len++] = token[i];
                }
            }
            clean_word[clean_len] = '\0';
            
            if (clean_len > 0) {
                WordCount *existing = find_word(words_stack, word_count, clean_word);
                if (existing != NULL) {
                    existing->count++;
                    free(clean_word);
                } else {
                    words_stack[word_count].word = clean_word;
                    words_stack[word_count].count = 1;
                    word_heap[heap_word_count++] = clean_word;
                    word_count++;
                }
            } else {
                free(clean_word);
            }
        }
        token = strtok(NULL, " ");
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top 10):\n");
    int limit = word_count < 10 ? word_count : 10;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    free(lower_input);
    for (int i = 0; i < heap_word_count; i++) {
        free(word_heap[i]);
    }
    
    return 0;
}