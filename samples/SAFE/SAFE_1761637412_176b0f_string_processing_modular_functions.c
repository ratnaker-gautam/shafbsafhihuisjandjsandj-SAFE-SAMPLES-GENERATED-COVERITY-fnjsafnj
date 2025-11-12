//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORD_COUNT 512

struct WordFrequency {
    char word[64];
    int count;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_INPUT_LENGTH) return 0;
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_character(char c) {
    return isalpha((unsigned char)c) || c == '\'';
}

int extract_words(const char* text, char words[][64], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    
    while (text[i] != '\0' && word_count < max_words) {
        while (text[i] != '\0' && !is_word_character(text[i])) {
            i++;
        }
        
        if (text[i] == '\0') break;
        
        size_t start = i;
        while (text[i] != '\0' && is_word_character(text[i])) {
            i++;
        }
        
        size_t length = i - start;
        if (length >= 63) length = 62;
        
        strncpy(words[word_count], &text[start], length);
        words[word_count][length] = '\0';
        word_count++;
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

int compare_frequencies(const void* a, const void* b) {
    const struct WordFrequency* fa = (const struct WordFrequency*)a;
    const struct WordFrequency* fb = (const struct WordFrequency*)b;
    
    if (fb->count != fa->count) {
        return fb->count - fa->count;
    }
    return strcmp(fa->word, fb->word);
}

int analyze_word_frequency(const char* text, struct WordFrequency* frequencies, int max_frequencies) {
    if (text == NULL || frequencies == NULL || max_frequencies <= 0) return 0;
    
    char words[MAX_WORD_COUNT][64];
    int word_count = extract_words(text, words, MAX_WORD_COUNT);
    
    if (word_count <= 0) return 0;
    
    int freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
        
        if (strlen(words[i]) == 0) continue;
        
        int index = find_word_index(frequencies, freq_count, words[i]);
        
        if (index >= 0) {
            if (frequencies[index].count < 1000000) {
                frequencies[index].count++;
            }
        } else if (freq_count < max_frequencies) {
            strncpy(frequencies[freq_count].word, words[i], 63);
            frequencies[freq_count].word[63] = '\0';
            frequencies[freq_count].count = 1;
            freq_count++;
        }
    }
    
    qsort(frequencies, freq_count, sizeof(struct WordFrequency), compare_frequencies);
    return freq_count;
}

void print_frequencies(struct WordFrequency* frequencies, int count) {
    if (frequencies == NULL || count <= 0) return;
    
    printf("Word Frequency Analysis:\n");
    printf("========================\n");
    
    int display_count = count > 10 ? 10 : count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", frequencies[i].word, frequencies[i].count);
    }
    
    if (count > 10) {
        printf("... and %d more words\n", count - 10);
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int freq_count = analyze_word_frequency(input, frequencies, MAX_WORD_COUNT);
    
    if (freq_count <= 0) {
        printf("No words found in the input\n");
        return 0;
    }
    
    print_frequencies(frequencies, freq_count);
    
    return 0;
}