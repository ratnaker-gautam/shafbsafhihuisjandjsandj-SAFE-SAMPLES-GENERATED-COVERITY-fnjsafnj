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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    strcpy(copy, s);
    return copy;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount *word_counts = NULL;
    int word_count_size = 0;
    int word_count_capacity = 0;
    
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
        return 0;
    }
    
    char *word_buffer = malloc(MAX_INPUT_LEN + 1);
    if (word_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_len = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_len < MAX_INPUT_LEN) {
            word_buffer[word_len++] = tolower((unsigned char)c);
            in_word = 1;
        } else {
            if (in_word && word_len > 0) {
                word_buffer[word_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < word_count_size; j++) {
                    if (strcmp(word_counts[j].word, word_buffer) == 0) {
                        word_counts[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    if (word_count_size >= word_count_capacity) {
                        int new_capacity = word_count_capacity == 0 ? 10 : word_count_capacity * 2;
                        if (new_capacity > MAX_WORDS) new_capacity = MAX_WORDS;
                        
                        WordCount *new_counts = realloc(word_counts, new_capacity * sizeof(WordCount));
                        if (new_counts == NULL) {
                            fprintf(stderr, "Memory allocation failed\n");
                            free(word_buffer);
                            for (int j = 0; j < word_count_size; j++) free(word_counts[j].word);
                            free(word_counts);
                            return 1;
                        }
                        word_counts = new_counts;
                        word_count_capacity = new_capacity;
                    }
                    
                    char *word_copy = strdup_safe(word_buffer);
                    if (word_copy == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(word_buffer);
                        for (int j = 0; j < word_count_size; j++) free(word_counts[j].word);
                        free(word_counts);
                        return 1;
                    }
                    
                    word_counts[word_count_size].word = word_copy;
                    word_counts[word_count_size].count = 1;
                    word_count_size++;
                }
                
                word_len = 0;
                in_word = 0;
            }
        }
    }
    
    free(word_buffer);
    
    if (word_count_size > 0) {
        qsort(word_counts, word_count_size, sizeof(WordCount), compare_word_counts);
        
        printf("\nWord frequency analysis:\n");
        printf("------------------------\n");
        for (int i = 0; i < word_count_size && i < 10; i++) {
            printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    } else {
        printf("No valid words found in input\n");
    }
    
    for (int i = 0; i < word_count_size; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}