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
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_text(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return -1;
    strcpy(buffer, input);
    
    char *token = buffer;
    int pos = 0;
    
    while (*token && *word_count < MAX_WORDS) {
        while (*token && !is_valid_char(*token)) token++;
        if (!*token) break;
        
        char *start = token;
        while (*token && is_valid_char(*token)) token++;
        
        int len = token - start;
        if (len >= MAX_WORD_LEN) continue;
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, start, len);
        current_word[len] = '\0';
        
        for (int i = 0; current_word[i]; i++) {
            current_word[i] = tolower((unsigned char)current_word[i]);
        }
        
        int found = 0;
        for (int i = 0; i < *word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                if (words[i].count < INT_MAX) words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strcpy(words[*word_count].word, current_word);
            words[*word_count].count = 1;
            (*word_count)++;
        }
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

int main() {
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
    
    if (process_text(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}