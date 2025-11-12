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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(input[i])) i++;
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(input[i]) && (i - start) < MAX_WORD_LEN - 1) i++;
        
        size_t word_len = i - start;
        if (word_len > 0) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, struct WordCount *freq, int *freq_count) {
    if (words == NULL || freq == NULL || freq_count == NULL) return;
    
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
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

void print_word_frequencies(struct WordCount *freq, int freq_count) {
    if (freq == NULL || freq_count <= 0) return;
    
    printf("Word frequencies (sorted by count):\n");
    for (int i = 0; i < freq_count; i++) {
        printf("%s: %d\n", freq[i].word, freq[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount frequencies[MAX_WORDS];
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
        printf("No words found in input\n");
        return 0;
    }
    
    normalize_words(words, word_count);
    count_word_frequency(words, word_count, frequencies, &freq_count);
    
    if (freq_count > 0) {
        qsort(frequencies, (size_t)freq_count, sizeof(struct WordCount), compare_word_counts);
        print_word_frequencies(frequencies, freq_count);
    }
    
    return 0;
}