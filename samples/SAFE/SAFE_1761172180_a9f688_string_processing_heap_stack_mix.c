//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *to_lower_case(const char *str) {
    if (!str) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (!result) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
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
    char buffer[MAX_WORD_LEN];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (!fgets(input, sizeof(input), stdin)) {
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
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (!word_counts) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    size_t pos = 0;
    int in_word = 0;
    int buf_pos = 0;
    
    while (pos <= input_len && buf_pos < MAX_WORD_LEN - 1) {
        char c = input[pos];
        
        if (is_valid_char(c)) {
            if (!in_word) {
                in_word = 1;
                buf_pos = 0;
            }
            buffer[buf_pos++] = c;
        } else {
            if (in_word) {
                buffer[buf_pos] = '\0';
                
                if (buf_pos > 0) {
                    char *lower_word = to_lower_case(buffer);
                    if (!lower_word) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(word_counts);
                        return 1;
                    }
                    
                    WordCount *existing = find_word(word_counts, word_count, lower_word);
                    if (existing) {
                        existing->count++;
                        free(lower_word);
                    } else {
                        if (word_count < MAX_WORDS) {
                            word_counts[word_count].word = lower_word;
                            word_counts[word_count].count = 1;
                            word_count++;
                        } else {
                            free(lower_word);
                        }
                    }
                }
                in_word = 0;
                buf_pos = 0;
            }
        }
        pos++;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        free(word_counts);
        return 1;
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}