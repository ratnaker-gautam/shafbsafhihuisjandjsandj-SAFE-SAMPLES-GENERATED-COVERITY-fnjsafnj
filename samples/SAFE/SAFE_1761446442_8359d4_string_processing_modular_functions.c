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

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t input_len = strlen(input);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && !is_word_char(input[i])) i++;
        
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && is_word_char(input[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(words[word_count], input + start, word_len);
        words[word_count][word_len] = '\0';
        
        word_count++;
    }
    
    return word_count;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
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

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* unique_words) {
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, words[i], MAX_WORD_LEN - 1);
        current_word[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(current_word);
        
        if (strlen(current_word) == 0) continue;
        
        int index = find_word_index(unique_words, unique_count, current_word);
        if (index == -1) {
            if (unique_count < MAX_WORDS) {
                strncpy(unique_words[unique_count].word, current_word, MAX_WORD_LEN - 1);
                unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                unique_words[unique_count].count = 1;
                unique_count++;
            }
        } else {
            unique_words[index].count++;
        }
    }
    
    return unique_count;
}

void print_word_counts(struct WordCount* word_counts, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
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
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    print_word_counts(unique_words, unique_count);
    
    return 0;
}