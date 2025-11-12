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
    int length;
} WordInfo;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int process_input(const char* input, WordInfo* words, int* word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    int len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    
    *word_count = 0;
    int pos = 0;
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && !is_valid_char(input[pos])) pos++;
        if (pos >= len) break;
        
        int start = pos;
        while (pos < len && is_valid_char(input[pos])) pos++;
        int end = pos;
        
        int word_len = end - start;
        if (word_len <= 0 || word_len >= MAX_WORD_LEN) continue;
        
        char temp_word[MAX_WORD_LEN];
        if (word_len >= MAX_WORD_LEN) continue;
        
        strncpy(temp_word, input + start, word_len);
        temp_word[word_len] = '\0';
        
        for (int i = 0; i < word_len; i++) {
            temp_word[i] = tolower((unsigned char)temp_word[i]);
        }
        
        int found = 0;
        for (int i = 0; i < *word_count; i++) {
            if (strcmp(words[i].word, temp_word) == 0) {
                if (words[i].count < INT_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (*word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                words[*word_count].length = word_len;
                (*word_count)++;
            }
        }
    }
    
    return 1;
}

void bubble_sort_words(WordInfo* words, int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                WordInfo temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    bubble_sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %-10s %-10s\n", "Word", "Count", "Length");
    printf("----------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (i < 20) {
            printf("%-20s %-10d %-10d\n", words[i].word, words[i].count, words[i].length);
        }
    }
    
    if (word_count > 20) {
        printf("... and %d more words\n", word_count - 20);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}