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

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char* text, struct WordCount words[], int* word_count) {
    if (text == NULL || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN) return 0;
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lowercase(buffer);
    
    *word_count = 0;
    size_t i = 0;
    while (buffer[i] != '\0' && *word_count < MAX_WORDS) {
        while (buffer[i] != '\0' && !is_word_char(buffer[i])) {
            i++;
        }
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && is_word_char(buffer[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN) continue;
        
        strncpy(words[*word_count].word, &buffer[start], word_len);
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

void sort_words_by_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_frequencies(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    printf("-----------------\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
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
        printf("Error processing words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    word_count = count_word_frequency(words, word_count);
    sort_words_by_f