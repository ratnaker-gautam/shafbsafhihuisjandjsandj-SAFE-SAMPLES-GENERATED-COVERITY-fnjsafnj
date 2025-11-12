//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char* input, char words[][64], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !isalpha((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && isalpha((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len >= 64) word_len = 63;
        
        for (size_t j = 0; j < word_len; j++) {
            words[word_count][j] = tolower((unsigned char)input[start + j]);
        }
        words[word_count][word_len] = '\0';
        
        if (word_len > 0) {
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(char words[][64], int word_count, struct WordCount* unique_words) {
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], 63);
            unique_words[unique_count].word[63] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
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

void print_word_stats(struct WordCount* unique_words, int unique_count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < unique_count; i++) {
        printf("%-15s: %d\n", unique_words[i].word, unique_words[i].count);
    }
    
    printf("Total unique words: %d\n", unique_count);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
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
        printf("Invalid input detected.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    if (unique_count == 0) {
        printf("Error processing word counts.\n");
        return 1;
    }
    
    sort_word_counts(unique_words, unique_count);
    print_word_stats(unique_words, unique_count);
    
    return 0;
}