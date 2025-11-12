//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_WORDS 1000
#define MAX_WORD_LENGTH 100

typedef struct {
    char word[MAX_WORD_LENGTH];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_word(const char *word, WordCount *words, int *word_count) {
    if (word == NULL || strlen(word) == 0) return 0;
    
    char processed[MAX_WORD_LENGTH];
    size_t len = strlen(word);
    if (len >= MAX_WORD_LENGTH) return 0;
    
    strncpy(processed, word, MAX_WORD_LENGTH - 1);
    processed[MAX_WORD_LENGTH - 1] = '\0';
    
    for (size_t i = 0; processed[i]; i++) {
        processed[i] = tolower((unsigned char)processed[i]);
    }
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, processed) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 1;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(words[*word_count].word, processed, MAX_WORD_LENGTH - 1);
        words[*word_count].word[MAX_WORD_LENGTH - 1] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
        return 1;
    }
    
    return 0;
}

void sort_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
               (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LENGTH - 1);
    
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
    
    char current_word[MAX_WORD_LENGTH];
    size_t word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < MAX_WORD_LENGTH - 1) {
            current_word[word_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                if (!process_word(current_word, words, &word_count)) {
                    fprintf(stderr, "Error processing word or word limit reached\n");
                }
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}