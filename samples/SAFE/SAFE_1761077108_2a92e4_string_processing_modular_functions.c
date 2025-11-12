//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        char c = input[i];
        if (isalnum(c) && !in_word) {
            word_start = i;
            in_word = 1;
        } else if (!isalnum(c) && in_word) {
            size_t word_len = i - word_start;
            if (word_len > 0 && word_len < MAX_WORD_LEN) {
                strncpy(words[word_count], input + word_start, word_len);
                words[word_count][word_len] = '\0';
                word_count++;
            }
            in_word = 0;
        }
    }
    
    return word_count;
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, WordCount* word_counts, int* unique_count) {
    if (words == NULL || word_counts == NULL || unique_count == NULL) return;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && *unique_count < MAX_WORDS) {
            strncpy(word_counts[*unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
}

void print_word_frequencies(const WordCount* word_counts, int unique_count) {
    if (word_counts == NULL) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count, unique_count;
    
    printf("Enter text to analyze: ");
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
    
    word_count = extract_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    process_words(words, word_count);
    count_word_frequency(words, word_count, word_counts, &unique_count);
    print_word_frequencies(word_counts, unique_count);
    
    return 0;
}