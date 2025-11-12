//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char* text, struct WordCount words[], int* word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t len = strlen(text);
    size_t i = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i]) && (i - start) < MAX_WORD_LEN - 1) i++;
        
        size_t word_len = i - start;
        if (word_len > 0) {
            for (size_t j = 0; j < word_len; j++) {
                words[*word_count].word[j] = tolower((unsigned char)text[start + j]);
            }
            words[*word_count].word[word_len] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 1;
}

int find_word_index(const struct WordCount words[], int word_count, const char* word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

int count_unique_words(const char* text, struct WordCount words[], int* unique_count) {
    if (!is_valid_input(text) || words == NULL || unique_count == NULL) return 0;
    
    int temp_count = 0;
    if (!extract_words(text, words, &temp_count)) return 0;
    
    *unique_count = 0;
    
    for (int i = 0; i < temp_count; i++) {
        int found_idx = find_word_index(words, *unique_count, words[i].word);
        if (found_idx >= 0) {
            if (words[found_idx].count < INT_MAX) {
                words[found_idx].count++;
            }
        } else {
            if (*unique_count < MAX_WORDS) {
                strncpy(words[*unique_count].word, words[i].word, MAX_WORD_LEN - 1);
                words[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*unique_count].count = 1;
                (*unique_count)++;
            }
        }
    }
    
    return 1;
}

void print_word_counts(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int compare_word_counts(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    return strcmp(wa->word, wb->word);
}

void sort_words_alphabetically(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    qsort(words, count, sizeof(struct WordCount), compare_word_counts);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
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
    
    if (!count_unique_words(input, words, &unique_count)) {
        printf("Error processing text.\n");
        return 1;
    }
    
    if (unique_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    sort_words_alphabetically(words, unique_count);
    print_word_counts(words, unique_count);
    
    return 0;
}