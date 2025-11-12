//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char* text, struct WordCount words[], int* word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t len = strlen(text);
    size_t i = 0;
    
    while (i < len && *word_count < MAX_WORDS) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i]) && (i - start) < MAX_WORD_LEN - 1) i++;
        
        size_t word_len = i - start;
        if (word_len > 0) {
            strncpy(words[*word_count].word, text + start, word_len);
            words[*word_count].word[word_len] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 1;
}

int find_word_index(const struct WordCount words[], int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void count_word_frequency(struct WordCount words[], int* word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) return;
    
    for (int i = 0; i < *word_count; i++) {
        for (int j = i + 1; j < *word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < *word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                (*word_count)--;
                j--;
            }
        }
    }
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void normalize_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    for (int i = 0; i < count; i++) {
        to_lowercase(words[i].word);
    }
}

void print_word_frequencies(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze: ");
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
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error extracting words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    normalize_words(words, word_count);
    count_word_frequency(words, &word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}