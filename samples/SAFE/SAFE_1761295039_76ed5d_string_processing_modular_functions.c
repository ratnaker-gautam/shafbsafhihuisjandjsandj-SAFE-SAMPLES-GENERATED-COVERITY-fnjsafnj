//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    size_t i = 0;
    size_t input_len = strlen(input);
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && !isalpha(input[i])) i++;
        
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && isalpha(input[i])) i++;
        
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

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* unique_words, int* unique_count) {
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    int unique_idx = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_idx; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_idx < MAX_WORDS) {
            strncpy(unique_words[unique_idx].word, words[i], MAX_WORD_LEN - 1);
            unique_words[unique_idx].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_idx].count = 1;
            unique_idx++;
        }
    }
    
    *unique_count = unique_idx;
    return 1;
}

void sort_word_counts(struct WordCount* unique_words, int unique_count) {
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (unique_words[j].count < unique_words[j + 1].count) {
                struct WordCount temp = unique_words[j];
                unique_words[j] = unique_words[j + 1];
                unique_words[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(const struct WordCount* unique_words, int unique_count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < unique_count; i++) {
        printf("%-20s: %d\n", unique_words[i].word, unique_words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_words[MAX_WORDS];
    
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
        printf("Invalid input: too long or empty.\n");
        return 1;
    }
    
    int word_count = 0;
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int unique_count = 0;
    if (!count_unique_words(words, word_count, unique_words, &unique_count)) {
        printf("Error counting unique words.\n");
        return 1;
    }
    
    sort_word_counts(unique_words, unique_count);
    print_word_counts(unique_words, unique_count);
    
    return 0;
}