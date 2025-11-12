//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50

struct WordCount {
    char word[MAX_WORD_LENGTH];
    int count;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LENGTH) return 0;
    return 1;
}

int is_word_character(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char* input, char words[][MAX_WORD_LENGTH], int max_words) {
    if (!is_valid_input(input) || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_character(input[i])) {
            i++;
        }
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_character(input[i]) && (i - start) < MAX_WORD_LENGTH - 1) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LENGTH) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount* word_counts, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LENGTH], int word_count, struct WordCount* unique_words) {
    if (word_count <= 0 || unique_words == NULL) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LENGTH];
        strncpy(temp_word, words[i], MAX_WORD_LENGTH - 1);
        temp_word[MAX_WORD_LENGTH - 1] = '\0';
        to_lowercase(temp_word);
        
        int index = find_word_index(unique_words, unique_count, temp_word);
        if (index == -1) {
            if (unique_count < MAX_WORDS) {
                strncpy(unique_words[unique_count].word, temp_word, MAX_WORD_LENGTH - 1);
                unique_words[unique_count].word[MAX_WORD_LENGTH - 1] = '\0';
                unique_words[unique_count].count = 1;
                unique_count++;
            }
        } else {
            if (unique_words[index].count < INT_MAX) {
                unique_words[index].count++;
            }
        }
    }
    
    return unique_count;
}

void print_word_counts(struct WordCount* word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int compare_word_counts(const void* a, const void* b) {
    const struct WordCount* wa = (const struct WordCount*)a;
    const struct WordCount* wb = (const struct WordCount*)b;
    
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void sort_word_counts(struct WordCount* word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    qsort(word_counts, count, sizeof(struct WordCount), compare_word_counts);
}

int main(void) {
    char input[MAX_INPUT_LENGTH + 1];
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    struct WordCount unique_words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input.\n");
        return