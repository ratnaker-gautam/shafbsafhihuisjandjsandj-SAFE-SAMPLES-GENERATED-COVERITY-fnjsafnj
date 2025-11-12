//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 512

struct WordFrequency {
    char word[64];
    int count;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    if (strlen(input) >= MAX_INPUT_LENGTH) return 0;
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_character(char c) {
    return isalpha((unsigned char)c);
}

int extract_words(const char* input, char words[][64], int max_words) {
    if (!is_valid_input(input) || max_words <= 0) return 0;
    
    char buffer[MAX_INPUT_LENGTH];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_lowercase(buffer);
    
    int word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && word_count < max_words) {
        while (buffer[i] != '\0' && !is_word_character(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && is_word_character(buffer[i])) {
            i++;
        }
        
        size_t length = i - start;
        if (length > 0 && length < 64) {
            strncpy(words[word_count], &buffer[start], length);
            words[word_count][length] = '\0';
            word_count++;
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

int count_word_frequencies(char words[][64], int word_count, struct WordFrequency* frequencies) {
    if (word_count <= 0 || frequencies == NULL) return 0;
    
    int freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(frequencies, freq_count, words[i]);
        if (index >= 0) {
            if (frequencies[index].count < INT_MAX) {
                frequencies[index].count++;
            }
        } else if (freq_count < MAX_WORD_COUNT) {
            strncpy(frequencies[freq_count].word, words[i], 63);
            frequencies[freq_count].word[63] = '\0';
            frequencies[freq_count].count = 1;
            freq_count++;
        }
    }
    
    return freq_count;
}

void sort_frequencies(struct WordFrequency* frequencies, int count) {
    if (frequencies == NULL || count <= 1) return;
    
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
    if (frequencies == NULL || count <= 0) return;
    
    printf("Word frequencies (top 10):\n");
    printf("---------------------------\n");
    
    int limit = count > 10 ? 10 : count;
    for (int i = 0; i < limit; i++) {
        printf("%-15s: %d\n", frequencies[i].word, frequencies[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    char words[MAX_WORD_COUNT][64];
    struct WordFrequency frequencies[MAX_WORD_COUNT];
    
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
    
    int word_count = extract_words(input, words, MAX_WORD_COUNT);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int freq_count = count_word_frequencies(words, word_count, frequencies);
    if (freq_count == 0) {
        printf("No word frequencies calculated\n");
        return 0;
    }
    
    sort_frequencies(frequencies, freq_count);
    print