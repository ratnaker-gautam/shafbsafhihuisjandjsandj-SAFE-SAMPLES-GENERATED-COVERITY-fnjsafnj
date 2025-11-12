//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && !isalpha(input[i])) i++;
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && isalpha(input[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN) continue;
        
        for (size_t j = 0; j < word_len; j++) {
            words[count][j] = tolower(input[start + j]);
        }
        words[count][word_len] = '\0';
        count++;
    }
    
    *word_count = count;
    return 1;
}

int count_word_frequency(char words[][MAX_WORD_LEN], int word_count, WordCount* word_counts, int* unique_count) {
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    int unique = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique < MAX_WORDS) {
            strncpy(word_counts[unique].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique].count = 1;
            unique++;
        }
    }
    
    *unique_count = unique;
    return 1;
}

void sort_word_counts(WordCount* word_counts, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (word_counts[j].count < word_counts[j + 1].count) {
                WordCount temp = word_counts[j];
                word_counts[j] = word_counts[j + 1];
                word_counts[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error extracting words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    if (!count_word_frequency(words, word_count, word_counts, &unique_count)) {
        printf("Error counting word frequency.\n");
        return 1;
    }
    
    sort_word_counts(word_counts, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    int display_count = unique_count > 10 ? 10 : unique_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    return 0;
}