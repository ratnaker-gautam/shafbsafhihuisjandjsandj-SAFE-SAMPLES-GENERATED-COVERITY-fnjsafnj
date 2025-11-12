//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words) {
    char buffer[MAX_INPUT_LEN + 1];
    if (strlen(input) >= MAX_INPUT_LEN) {
        return -1;
    }
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    int word_count = 0;
    int pos = 0;
    int len = strlen(buffer);
    
    while (pos < len && word_count < MAX_WORDS) {
        while (pos < len && !is_valid_char(buffer[pos])) {
            pos++;
        }
        
        if (pos >= len) break;
        
        int start = pos;
        while (pos < len && is_valid_char(buffer[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        char current_word[MAX_WORD_LEN + 1];
        strncpy(current_word, &buffer[start], word_len);
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        int existing_index = find_word(words, word_count, current_word);
        if (existing_index != -1) {
            if (words[existing_index].count < INT32_MAX) {
                words[existing_index].count++;
            }
        } else {
            strncpy(words[word_count].word, current_word, MAX_WORD_LEN);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void sort_words(WordCount *words, int count) {
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

int main() {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = process_input(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
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