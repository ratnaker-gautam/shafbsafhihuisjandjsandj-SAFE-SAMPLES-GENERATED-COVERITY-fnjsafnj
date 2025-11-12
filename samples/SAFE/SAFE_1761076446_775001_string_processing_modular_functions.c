//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][64], int max_words) {
    if (!is_valid_input(input) || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        unsigned char c = (i < input_len) ? (unsigned char)input[i] : ' ';
        
        if (isalnum(c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < 64) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

void count_word_frequency(char words[][64], int word_count, struct WordCount *freq, int *freq_count) {
    if (words == NULL || freq == NULL || freq_count == NULL || word_count <= 0) return;
    
    *freq_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *freq_count; j++) {
            if (strcmp(words[i], freq[j].word) == 0) {
                freq[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && *freq_count < MAX_WORDS) {
            strncpy(freq[*freq_count].word, words[i], 63);
            freq[*freq_count].word[63] = '\0';
            freq[*freq_count].count = 1;
            (*freq_count)++;
        }
    }
}

void sort_word_frequency(struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 1) return;
    
    for (int i = 0; i < freq_count - 1; i++) {
        for (int j = 0; j < freq_count - i - 1; j++) {
            if (freq[j].count < freq[j + 1].count) {
                struct WordCount temp = freq[j];
                freq[j] = freq[j + 1];
                freq[j + 1] = temp;
            }
        }
    }
}

void display_results(struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < freq_count; i++) {
        printf("%-20s: %d\n", freq[i].word, freq[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    struct WordCount frequency[MAX_WORDS];
    int word_count, freq_count;
    
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
    
    word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    count_word_frequency(words, word_count, frequency, &freq_count);
    sort_word_frequency(frequency, freq_count);
    display_results(frequency, freq_count);
    
    return 0;
}