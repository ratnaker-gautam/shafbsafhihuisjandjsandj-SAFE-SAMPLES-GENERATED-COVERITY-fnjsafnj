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
    int first_position;
} WordInfo;

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int find_or_add_word(WordInfo words[], int *word_count, const char *word, int position) {
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return i;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
        words[*word_count].count = 1;
        words[*word_count].first_position = position;
        (*word_count)++;
        return *word_count - 1;
    }
    return -1;
}

void process_text(const char *text, WordInfo words[], int *word_count) {
    *word_count = 0;
    int len = strlen(text);
    int pos = 0;
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && !is_word_char(text[pos])) {
            pos++;
        }
        
        if (pos >= len) break;
        
        int start = pos;
        while (pos < len && is_word_char(text[pos])) {
            pos++;
        }
        
        int word_len = pos - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char word[MAX_WORD_LEN];
            strncpy(word, &text[start], word_len);
            word[word_len] = '\0';
            
            for (int i = 0; i < word_len; i++) {
                word[i] = tolower((unsigned char)word[i]);
            }
            
            find_or_add_word(words, word_count, word, start);
        }
    }
}

int compare_words(const void *a, const void *b) {
    const WordInfo *wa = (const WordInfo *)a;
    const WordInfo *wb = (const WordInfo *)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return wa->first_position - wb->first_position;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordInfo words[MAX_WORDS];
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
        printf("No input provided\n");
        return 1;
    }
    
    if (strlen(input) > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordInfo), compare_words);
    
    printf("\nWord frequency analysis (sorted by frequency, then by first occurrence):\n");
    printf("%-20s %-10s %s\n", "Word", "Count", "First Position");
    printf("------------------------------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %-10d %d\n", words[i].word, words[i].count, words[i].first_position);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    
    printf("\nTotal unique words: %d\n", word_count);
    printf("Total word occurrences: %d\n", total_words);
    
    return 0;
}