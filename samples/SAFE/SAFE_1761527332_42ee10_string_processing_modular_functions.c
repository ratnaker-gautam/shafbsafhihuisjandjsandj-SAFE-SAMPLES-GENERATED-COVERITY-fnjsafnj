//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char* text, WordCount words[], int* word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t len = strlen(text);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = text[i];
        if (is_word_char(c) && buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
        } else if (buf_pos > 0) {
            buffer[buf_pos] = '\0';
            
            int found = 0;
            for (int j = 0; j < *word_count; j++) {
                if (strcmp(words[j].word, buffer) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found && *word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, buffer, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
            
            buf_pos = 0;
        }
        
        if (*word_count >= MAX_WORDS) break;
    }
    
    return 1;
}

int compare_word_counts(const void* a, const void* b) {
    const WordCount* wa = (const WordCount*)a;
    const WordCount* wb = (const WordCount*)b;
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void sort_words(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    qsort(words, word_count, sizeof(WordCount), compare_word_counts);
}

void print_word_stats(WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        printf("No words to display.\n");
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("=======================\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input detected.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    print_word_stats(words, word_count);
    
    return 0;
}