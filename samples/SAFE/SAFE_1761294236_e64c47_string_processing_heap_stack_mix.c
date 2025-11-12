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
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char buffer[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *token = input;
    
    while (*token != '\0' && word_count < MAX_WORDS) {
        while (*token != '\0' && !is_valid_char(*token)) {
            token++;
        }
        
        if (*token == '\0') break;
        
        char *word_start = token;
        while (*token != '\0' && is_valid_char(*token)) {
            token++;
        }
        
        size_t word_len = token - word_start;
        if (word_len == 0) continue;
        
        if (word_len >= sizeof(buffer)) {
            fprintf(stderr, "Word too long\n");
            free(word_counts);
            return 1;
        }
        
        memcpy(buffer, word_start, word_len);
        buffer[word_len] = '\0';
        
        for (char *p = buffer; *p; p++) {
            *p = tolower((unsigned char)*p);
        }
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(word_counts[i].word, buffer) == 0) {
                word_counts[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            char *word_copy = strdup_safe(buffer);
            if (word_copy == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(word_counts[i].word);
                }
                free(word_counts);
                return 1;
            }
            word_counts[word_count].word = word_copy;
            word_counts[word_count].count = 1;
            word_count++;
        }
    }
    
    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequencies (alphabetical order):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}