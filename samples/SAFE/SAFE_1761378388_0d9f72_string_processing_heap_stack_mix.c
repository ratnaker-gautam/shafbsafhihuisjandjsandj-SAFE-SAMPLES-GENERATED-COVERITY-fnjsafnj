//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char* to_lowercase(const char* str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len >= MAX_WORD_LEN) return NULL;
    
    char* result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    
    return result;
}

int find_word_index(WordCount* words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_count(const void* a, const void* b) {
    const WordCount* wa = (const WordCount*)a;
    const WordCount* wb = (const WordCount*)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[1024];
    WordCount* word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    
    printf("Enter text (max 1023 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(word_counts);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        free(word_counts);
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    char* current_word = malloc(MAX_WORD_LEN);
    if (current_word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(word_counts);
        return 1;
    }
    
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < MAX_WORD_LEN - 1) {
            current_word[word_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                
                char* lower_word = to_lowercase(current_word);
                if (lower_word != NULL) {
                    int index = find_word_index(word_counts, word_count, lower_word);
                    
                    if (index >= 0) {
                        word_counts[index].count++;
                    } else if (word_count < MAX_WORDS) {
                        strncpy(word_counts[word_count].word, lower_word, MAX_WORD_LEN - 1);
                        word_counts[word_count].word[MAX_WORD_LEN - 1] = '\0';
                        word_counts[word_count].count = 1;
                        word_count++;
                    }
                    
                    free(lower_word);
                }
                
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    free(current_word);
    
    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
        int display_count = word_count > 10 ? 10 : word_count;
        
        for (int i = 0; i < display_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    free(word_counts);
    return 0;
}