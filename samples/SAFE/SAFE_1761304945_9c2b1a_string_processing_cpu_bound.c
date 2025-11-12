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
    return isalnum((unsigned char)c) || c == '_';
}

int process_input(const char *input, WordCount *words, int *word_count) {
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
        while (pos < len && is_valid_char(input[pos]) && (pos - start) < MAX_WORD_LEN - 1) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char current_word[MAX_WORD_LEN];
            strncpy(current_word, &input[start], word_len);
            current_word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
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
                strcpy(words[*word_count].word, current_word);
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
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
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency analysis (sorted by frequency):\n");
    for (int i = 0; i < word_count && i < 20; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}