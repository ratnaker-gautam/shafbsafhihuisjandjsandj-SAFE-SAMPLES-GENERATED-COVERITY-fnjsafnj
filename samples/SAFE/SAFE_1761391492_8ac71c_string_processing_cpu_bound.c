//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 4096
#define MAX_WORDS 1000
#define MAX_WORD_LEN 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    strcpy(buffer, input);
    
    char *token = strtok(buffer, " \t\n\r");
    int count = 0;
    
    while (token != NULL && count < MAX_WORDS) {
        size_t len = strlen(token);
        if (len == 0 || len >= MAX_WORD_LEN) {
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        for (size_t i = 0; i < len; i++) {
            if (!is_valid_char(token[i])) {
                token = strtok(NULL, " \t\n\r");
                continue;
            }
        }
        
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i].word, token) == 0) {
                if (words[i].count < INT_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(words[count].word, token);
            words[count].count = 1;
            count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    *word_count = count;
    return 1;
}

void sort_words(WordCount *words, int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}