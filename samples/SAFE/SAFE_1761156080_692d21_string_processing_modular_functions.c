//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
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
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (!is_valid_input(text) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char *p = text;
    
    while (*p && word_count < max_words) {
        while (*p && !is_word_char(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        while (*p && is_word_char(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len >= sizeof(words[0].word)) word_len = sizeof(words[0].word) - 1;
        
        memcpy(words[word_count].word, start, word_len);
        words[word_count].word[word_len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

int find_word_index(const struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(struct WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) return;
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        int found_idx = find_word_index(words, unique_count, words[i].word);
        
        if (found_idx >= 0) {
            words[found_idx].count++;
        } else {
            if (unique_count < i) {
                words[unique_count] = words[i];
            }
            unique_count++;
        }
    }
    
    *word_count = unique_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
}

void normalize_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        to_lowercase(words[i].word);
    }
}

int compare_word_counts(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    
    if (wb->count != wa->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

void sort_word_frequencies(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    qsort(words, count, sizeof(struct WordCount), compare_word_counts);
}

void print_word_frequencies(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
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
    if (word_count <= 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    normalize_words(words, word_count);
    count_word_frequencies(words, &word_count);
    sort_word_frequencies(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}