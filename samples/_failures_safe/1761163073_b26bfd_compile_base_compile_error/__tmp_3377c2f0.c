//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 512
#define MAX_WORD_LENGTH 64

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

int extract_words(const char* input, char words[][MAX_WORD_LENGTH], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t input_len = strlen(input);
    
    while (i < input_len && word_count < max_words) {
        while (i < input_len && !is_word_character(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && is_word_character(input[i]) && (i - start) < MAX_WORD_LENGTH - 1) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0) {
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

int find_word_index(struct WordFrequency* frequencies, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(frequencies[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_word_frequencies(char words[][MAX_WORD_LENGTH], int word_count, struct WordFrequency* frequencies) {
    if (word_count <= 0) return 0;
    
    int freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LENGTH];
        strncpy(temp_word, words[i], MAX_WORD_LENGTH - 1);
        temp_word[MAX_WORD_LENGTH - 1] = '\0';
        to_lowercase(temp_word);
        
        int index = find_word_index(frequencies, freq_count, temp_word);
        if (index >= 0) {
            frequencies[index].count++;
        } else if (freq_count < MAX_WORD_COUNT) {
            strncpy(frequencies[freq_count].word, temp_word, MAX_WORD_LENGTH - 1);
            frequencies[freq_count].word[MAX_WORD_LENGTH - 1] = '\0';
            frequencies[freq_count].count = 1;
            freq_count++;
        }
    }
    
    return freq_count;
}

void sort_frequencies(struct WordFrequency* frequencies, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (frequencies[j].count < frequencies[j + 1].count) {
                struct WordFrequency temp = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp;
            }
        }
    }
}

void print_frequencies(struct WordFrequency* frequencies, int count) {
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-20s: %d\n", frequencies[i].word, frequencies[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH + 1];
    char words[MAX_WORD_COUNT][MAX_WORD_LENGTH];
    struct WordFrequency frequencies[MAX_WORD_COUNT];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LENGTH);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: empty or too long.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORD_COUNT);
    
    if (word_count == 0) {
        printf("No words found in the input.\n");
        return 0;
    }
    
    int freq_count = count_word_frequencies(words, word_count, frequencies);
    
    if (freq_count > 0) {
        sort_frequencies(frequencies, freq_count);
        print_frequencies(frequencies, freq_count);
    }