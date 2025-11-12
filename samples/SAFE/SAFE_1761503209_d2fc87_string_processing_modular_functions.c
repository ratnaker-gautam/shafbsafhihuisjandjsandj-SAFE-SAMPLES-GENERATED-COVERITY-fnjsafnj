//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(input[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(input[i]) && (i - start) < (size_t)(MAX_WORD_LEN - 1)) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < (size_t)MAX_WORD_LEN) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, struct WordCount *freq, int *freq_count) {
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
            strncpy(freq[*freq_count].word, words[i], MAX_WORD_LEN - 1);
            freq[*freq_count].word[MAX_WORD_LEN - 1] = '\0';
            freq[*freq_count].count = 1;
            (*freq_count)++;
        }
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void normalize_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
}

void print_word_frequency(struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < freq_count; i++) {
        printf("%s: %d\n", freq[i].word, freq[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount frequency[MAX_WORDS];
    int word_count, freq_count;
    
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
    
    word_count = extract_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    normalize_words(words, word_count);
    count_word_frequency(words, word_count, frequency, &freq_count);
    print_word_frequency(frequency, freq_count);
    
    return 0;
}