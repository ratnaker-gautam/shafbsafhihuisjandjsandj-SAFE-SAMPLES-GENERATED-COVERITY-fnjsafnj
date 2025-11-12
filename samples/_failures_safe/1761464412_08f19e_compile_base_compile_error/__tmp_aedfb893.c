//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'';
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int max_words) {
    if (input == NULL || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return 0;
    strcpy(buffer, input);
    to_lowercase(buffer);
    
    int word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && word_count < max_words) {
        while (buffer[i] != '\0' && !is_word_char(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && is_word_char(buffer[i])) {
            i++;
        }
        
        size_t length = i - start;
        if (length > 0 && length < MAX_WORD_LEN) {
            strncpy(words[word_count], &buffer[start], length);
            words[word_count][length] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

int find_word_index(struct WordCount* counts, int count, const char* word) {
    if (counts == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* unique_counts) {
    if (words == NULL || unique_counts == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(unique_counts, unique_count, words[i]);
        if (index == -1) {
            if (unique_count < MAX_WORDS) {
                strcpy(unique_counts[unique_count].word, words[i]);
                unique_counts[unique_count].count = 1;
                unique_count++;
            }
        } else {
            unique_counts[index].count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount* counts, int count) {
    if (counts == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (counts[j].count < counts[j + 1].count || 
                (counts[j].count == counts[j + 1].count && strcmp(counts[j].word, counts[j + 1].word) > 0)) {
                struct WordCount temp = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(struct WordCount* counts, int count) {
    if (counts == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", counts[i].word, counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_counts[MAX_WORDS];
    
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
        printf("Invalid input.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_counts);
    if (unique_count == 0)