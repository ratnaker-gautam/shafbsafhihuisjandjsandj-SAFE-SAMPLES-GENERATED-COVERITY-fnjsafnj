//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    if (strlen(input) >= sizeof(buffer)) {
        return -1;
    }
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = buffer;
    int count = 0;
    
    while (*token && count < MAX_WORDS) {
        while (*token && !is_valid_char(*token)) {
            token++;
        }
        
        if (!*token) break;
        
        char *start = token;
        while (*token && is_valid_char(*token)) {
            token++;
        }
        
        size_t word_len = token - start;
        if (word_len == 0 || word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, start, word_len);
        current_word[word_len] = '\0';
        
        for (char *p = current_word; *p; p++) {
            *p = tolower((unsigned char)*p);
        }
        
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                if (words[i].count < INT32_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (count < MAX_WORDS - 1) {
                strncpy(words[count].word, current_word, MAX_WORD_LEN - 1);
                words[count].word[MAX_WORD_LEN - 1] = '\0';
                words[count].count = 1;
                count++;
            }
        }
        
        if (!*token) break;
        token++;
    }
    
    *word_count = count;
    return 0;
}

void sort_words(WordCount *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}