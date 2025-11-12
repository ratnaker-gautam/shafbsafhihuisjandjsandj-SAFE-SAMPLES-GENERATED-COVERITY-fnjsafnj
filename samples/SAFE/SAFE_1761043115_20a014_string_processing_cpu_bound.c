//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    return isalnum((unsigned char)c) || c == '_';
}

int process_input(const char* input, WordCount* words, int* word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    int len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    *word_count = 0;
    int i = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        while (i < len && !is_valid_char(input[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_valid_char(input[i]) && (i - start) < MAX_WORD_LEN - 1) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char current_word[MAX_WORD_LEN];
            strncpy(current_word, input + start, word_len);
            current_word[word_len] = '\0';
            
            for (int j = 0; j < word_len; j++) {
                current_word[j] = tolower((unsigned char)current_word[j]);
            }
            
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, current_word) == 0) {
                    if (words[j].count < INT_MAX) {
                        words[j].count++;
                    }
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
    }
    
    return 0;
}

void sort_words(WordCount* words, int word_count) {
    if (words == NULL || word_count <= 1) {
        return;
    }
    
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
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}