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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
    int word_count = 0;
    
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
    
    char *current = input;
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_valid_char(*current)) {
            current++;
        }
        
        if (*current == '\0') break;
        
        char *word_start = current;
        while (*current != '\0' && is_valid_char(*current)) {
            *current = tolower((unsigned char)*current);
            current++;
        }
        
        size_t word_len = current - word_start;
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) free(words[i]);
                return 1;
            }
            memcpy(word, word_start, word_len);
            word[word_len] = '\0';
            words[word_count++] = word;
        }
        
        if (*current != '\0') current++;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    WordCount *word_counts = malloc(word_count * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < word_count; i++) free(words[i]);
        return 1;
    }
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            word_counts[unique_count].word = strdup_safe(words[i]);
            if (word_counts[unique_count].word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int k = 0; k < unique_count; k++) free(word_counts[k].word);
                for (int k = 0; k < word_count; k++) free(words[k]);
                free(word_counts);
                return 1;
            }
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    qsort(word_counts, unique_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top 10):\n");
    int display_count = unique_count > 10 ? 10 : unique_count;
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < unique_count; i++) {
        free(word_counts[i].word);
    }
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(word_counts);
    
    return 0;
}