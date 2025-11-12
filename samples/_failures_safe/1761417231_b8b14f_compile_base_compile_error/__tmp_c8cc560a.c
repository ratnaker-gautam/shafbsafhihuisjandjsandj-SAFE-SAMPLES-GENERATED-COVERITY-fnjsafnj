//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LENGTH 50

struct WordFrequency {
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

void to_lower_case(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char* input, char words[][MAX_WORD_LENGTH], int max_words) {
    if (!is_valid_input(input) || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        char current = (i < input_len) ? input[i] : ' ';
        
        if (is_word_character(current)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_length = i - word_start;
                if (word_length > 0 && word_length < MAX_WORD_LENGTH) {
                    strncpy(words[word_count], input + word_start, word_length);
                    words[word_count][word_length] = '\0';
                    to_lower_case(words[word_count]);
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int find_word_index(struct WordFrequency* frequencies, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(frequencies[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_word_frequencies(char words[][MAX_WORD_LENGTH], int word_count, 
                          struct WordFrequency* frequencies, int max_frequencies) {
    if (word_count <= 0 || max_frequencies <= 0) return 0;
    
    int freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(frequencies, freq_count, words[i]);
        if (index >= 0) {
            if (frequencies[index].count < INT_MAX) {
                frequencies[index].count++;
            }
        } else if (freq_count < max_frequencies) {
            strncpy(frequencies[freq_count].word, words[i], MAX_WORD_LENGTH - 1);
            frequencies[freq_count].word[MAX_WORD_LENGTH - 1] = '\0';
            frequencies[freq_count].count = 1;
            freq_count++;
        }
    }
    
    return freq_count;
}

void swap_frequencies(struct WordFrequency* a, struct WordFrequency* b) {
    struct WordFrequency temp = *a;
    *a = *b;
    *b = temp;
}

void sort_frequencies(struct WordFrequency* frequencies, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (frequencies[j].count < frequencies[j + 1].count) {
                swap_frequencies(&frequencies[j], &frequencies[j + 1]);
            }
        }
    }
}

void print_frequencies(struct WordFrequency* frequencies, int count) {
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", frequencies[i].word, frequencies[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH + 1];
    char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];
    struct WordFrequency frequencies[MAX_WORD_COUNT];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LENGTH);
    
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
    
    int word_count = extract_words(input, words, MAX_WORD_COUNT);
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int freq_count = count_word_frequencies