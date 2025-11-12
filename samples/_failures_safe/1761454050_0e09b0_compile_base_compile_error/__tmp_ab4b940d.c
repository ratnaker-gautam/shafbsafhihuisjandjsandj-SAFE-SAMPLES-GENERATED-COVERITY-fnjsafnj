//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (!is_valid_input(input)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN) return 0;
                if (*word_count >= MAX_WORDS) return 0;
                
                for (size_t j = 0; j < word_len; j++) {
                    words[*word_count][j] = tolower(input[word_start + j]);
                }
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

int count_word_frequency(char words[][MAX_WORD_LEN], int word_count, struct WordCount *freq, int *freq_count) {
    if (words == NULL || freq == NULL || freq_count == NULL) return 0;
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
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
        if (!found) {
            if (*freq_count >= MAX_WORDS) return 0;
            strncpy(freq[*freq_count].word, words[i], MAX_WORD_LEN - 1);
            freq[*freq_count].word[MAX_WORD_LEN - 1] = '\0';
            freq[*freq_count].count = 1;
            (*freq_count)++;
        }
    }
    return 1;
}

void sort_word_frequency(struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 0) return;
    
    for (int i = 0; i < freq_count - 1; i++) {
        for (int j = 0; j < freq_count - i - 1; j++) {
            if (freq[j].count < freq[j + 1].count || 
                (freq[j].count == freq[j + 1].count && strcmp(freq[j].word, freq[j + 1].word) > 0)) {
                struct WordCount temp = freq[j];
                freq[j] = freq[j + 1];
                freq[j + 1] = temp;
            }
        }
    }
}

void print_word_frequency(const struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < freq_count; i++) {
        printf("%-15s: %d\n", freq[i].word, freq[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount frequency[MAX_WORDS];
    int word_count = 0;
    int freq_count = 0;
    
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
        printf("Invalid input detected.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing words from input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    if (!count_word_frequency(words, word_count, frequency, &freq_count)) {
        printf("Error counting word