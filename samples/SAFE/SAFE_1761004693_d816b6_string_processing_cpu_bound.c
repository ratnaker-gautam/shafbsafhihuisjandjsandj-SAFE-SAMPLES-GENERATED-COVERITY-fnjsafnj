//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 4096
#define MAX_WORD_COUNT 1024

typedef struct {
    char word[64];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int process_text(const char *text, WordCount *words, int max_words) {
    char buffer[MAX_INPUT_LENGTH];
    char temp_word[64];
    int word_count = 0;
    
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    size_t text_len = strlen(text);
    if (text_len >= MAX_INPUT_LENGTH - 1) {
        return 0;
    }
    
    strncpy(buffer, text, MAX_INPUT_LENGTH - 1);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';
    
    for (size_t i = 0; i < text_len; i++) {
        buffer[i] = tolower((unsigned char)buffer[i]);
    }
    
    char *ptr = buffer;
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_valid_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len >= sizeof(temp_word) - 1) {
            continue;
        }
        
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        if (strlen(temp_word) < 2) {
            continue;
        }
        
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, temp_word) == 0) {
                if (words[i].count < INT_MAX) {
                    words[i].count++;
                }
                found = 1;
                break;
            }
        }
        
        if (!found && word_count < max_words) {
            strncpy(words[word_count].word, temp_word, sizeof(words[word_count].word) - 1);
            words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

int main(void) {
    char input_text[MAX_INPUT_LENGTH];
    WordCount words[MAX_WORD_COUNT];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LENGTH - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = process_text(input_text, words, MAX_WORD_COUNT);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis (top 10):\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    int display_count = (word_count < 10) ? word_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s\t%d\n", words[i].word, words[i].count);
    }
    
    return 0;
}