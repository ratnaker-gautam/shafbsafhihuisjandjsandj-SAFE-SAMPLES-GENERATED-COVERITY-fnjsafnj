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
    char buffer[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    int len = strlen(input);
    
    if (len >= MAX_INPUT_LEN - 1) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int i = 0;
    int buf_len = strlen(buffer);
    *word_count = 0;
    
    while (i < buf_len && *word_count < MAX_WORDS) {
        while (i < buf_len && !is_valid_char(buffer[i])) {
            i++;
        }
        
        if (i >= buf_len) {
            break;
        }
        
        int j = 0;
        while (i < buf_len && j < MAX_WORD_LEN - 1 && is_valid_char(buffer[i])) {
            temp_word[j++] = tolower((unsigned char)buffer[i++]);
        }
        temp_word[j] = '\0';
        
        if (j > 0) {
            int found = 0;
            for (int k = 0; k < *word_count; k++) {
                if (strcmp(words[k].word, temp_word) == 0) {
                    if (words[k].count < INT_MAX) {
                        words[k].count++;
                    }
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    
    return 0;
}

void sort_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
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
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (process_text(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    if (word_count > 20) {
        printf("... and %d more words\n", word_count - 20);
    }
    
    return 0;
}