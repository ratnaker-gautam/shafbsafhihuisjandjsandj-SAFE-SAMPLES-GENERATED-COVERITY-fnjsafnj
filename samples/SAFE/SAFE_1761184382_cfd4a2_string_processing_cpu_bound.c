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
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_input(const char *input, WordInfo *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    int len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) {
        return -1;
    }
    
    *word_count = 0;
    int pos = 0;
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && !is_valid_char(input[pos])) {
            pos++;
        }
        
        if (pos >= len) {
            break;
        }
        
        int start = pos;
        while (pos < len && is_valid_char(input[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len >= MAX_WORD_LEN || word_len == 0) {
            continue;
        }
        
        char current_word[MAX_WORD_LEN];
        if (word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        strncpy(current_word, input + start, word_len);
        current_word[word_len] = '\0';
        
        for (int i = 0; current_word[i]; i++) {
            current_word[i] = tolower((unsigned char)current_word[i]);
        }
        
        int found = -1;
        for (int i = 0; i < *word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                found = i;
                break;
            }
        }
        
        if (found >= 0) {
            if (words[found].count < INT_MAX) {
                words[found].count++;
            }
        } else {
            if (*word_count < MAX_WORDS) {
                strcpy(words[*word_count].word, current_word);
                words[*word_count].count = 1;
                words[*word_count].length = word_len;
                (*word_count)++;
            }
        }
    }
    
    return 0;
}

void sort_words(WordInfo *words, int word_count) {
    if (words == NULL || word_count <= 1) {
        return;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %-10s %-10s\n", "Word", "Count", "Length");
    printf("----------------------------------------\n");
    
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s %-10d %-10d\n", words[i].word, words[i].count, words[i].length);
    }
    
    if (word_count > 10) {
        printf("\n... and %d more words\n", word_count - 10);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        if (total_words <= INT_MAX - words[i].count) {
            total_words += words[i].count;
        }
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}