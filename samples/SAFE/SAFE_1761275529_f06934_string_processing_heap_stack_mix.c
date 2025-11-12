//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_word(char *word, WordCount *word_counts, int *word_count) {
    if (strlen(word) == 0) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            if (word_counts[i].count < INT_MAX) {
                word_counts[i].count++;
            }
            return 0;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(word_counts[*word_count].word, word, MAX_WORD_LEN - 1);
        word_counts[*word_count].word[MAX_WORD_LEN - 1] = '\0';
        word_counts[*word_count].count = 1;
        (*word_count)++;
        return 1;
    }
    
    return 0;
}

void process_text(const char *text, WordCount *word_counts, int *word_count) {
    char word[MAX_WORD_LEN];
    int word_pos = 0;
    int in_word = 0;
    size_t text_len = strlen(text);
    
    for (size_t i = 0; i < text_len; i++) {
        if (is_valid_char(text[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                word[word_pos++] = tolower((unsigned char)text[i]);
                in_word = 1;
            }
        } else {
            if (in_word) {
                word[word_pos] = '\0';
                process_word(word, word_counts, word_count);
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (in_word && word_pos > 0) {
        word[word_pos] = '\0';
        process_word(word, word_counts, word_count);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
}

int main(void) {
    char input_buffer[4096];
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    
    printf("Enter text (max 4095 characters): ");
    if (fgets(input_buffer, sizeof(input_buffer), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(word_counts);
        return 1;
    }
    
    size_t len = strlen(input_buffer);
    if (len > 0 && input_buffer[len - 1] == '\n') {
        input_buffer[len - 1] = '\0';
    }
    
    if (strlen(input_buffer) == 0) {
        printf("No input provided\n");
        free(word_counts);
        return 0;
    }
    
    process_text(input_buffer, word_counts, &word_count);
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequencies (sorted by count):\n");
    for (int i = 0; i < word_count; i++) {
        if (word_counts[i].count > 0) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    }
    
    free(word_counts);
    return 0;
}