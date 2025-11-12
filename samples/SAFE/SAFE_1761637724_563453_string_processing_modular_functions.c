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
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr && *word_count < MAX_WORDS) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *word_start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        char *word_end = ptr;
        
        size_t word_len = word_end - word_start;
        if (word_len >= sizeof(words[0].word)) word_len = sizeof(words[0].word) - 1;
        
        strncpy(words[*word_count].word, word_start, word_len);
        words[*word_count].word[word_len] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

int count_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) continue;
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
    
    int new_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            if (i != new_count) {
                words[new_count] = words[i];
            }
            new_count++;
        }
    }
    
    return new_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower(*p);
    }
}

void normalize_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) return wb->count - wa->count;
    return strcmp(wa->word, wb->word);
}

void sort_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
}

void print_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
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
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error extracting words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    normalize_words(words, word_count);
    word_count = count_word_frequency(words, word_count);
    sort_words(words, word_count);
    print_word_frequency(words, word_count);
    
    return 0;
}