//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(input[i])) i++;
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(input[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return word_count;
}

int find_word_index(WordCount* word_counts, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_word_frequency(char words[][MAX_WORD_LEN], int word_count, WordCount* word_counts) {
    if (word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(word_counts, unique_count, words[i]);
        if (index >= 0) {
            if (word_counts[index].count < INT_MAX) {
                word_counts[index].count++;
            }
        } else if (unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
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

void process_string_analysis(const char* input) {
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return;
    }
    
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return;
    }
    
    int unique_count = count_word_frequency(words, word_count, word_counts);
    
    sort_word_counts(word_counts, unique_count);
    
    printf("Word frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    printf("\nTop words by frequency:\n");
    
    int display_count = (unique_count > 10) ? 10 : unique_count;
    for (int i = 0; i < display_count; i++) {
        printf("%d. '%s' - %d occurrences\n", i + 1, word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_string_analysis(input);
    
    return 0;
}