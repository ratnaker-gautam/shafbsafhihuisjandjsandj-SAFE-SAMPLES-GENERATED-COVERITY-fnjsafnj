//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *text, char words[MAX_WORDS][64]) {
    if (text == NULL) return 0;
    int word_count = 0;
    size_t i = 0;
    while (text[i] != '\0' && word_count < MAX_WORDS) {
        while (text[i] != '\0' && !is_word_char(text[i])) i++;
        if (text[i] == '\0') break;
        size_t start = i;
        while (text[i] != '\0' && is_word_char(text[i]) && (i - start) < 63) i++;
        size_t len = i - start;
        if (len > 0) {
            strncpy(words[word_count], &text[start], len);
            words[word_count][len] = '\0';
            to_lowercase(words[word_count]);
            word_count++;
        }
    }
    return word_count;
}

int count_word_frequency(const char *word, char words[MAX_WORDS][64], int word_count) {
    if (word == NULL) return 0;
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word, words[i]) == 0) count++;
    }
    return count;
}

void remove_duplicates(char words[MAX_WORDS][64], int *word_count) {
    if (word_count == NULL || *word_count <= 0) return;
    int new_count = 0;
    for (int i = 0; i < *word_count; i++) {
        int found = 0;
        for (int j = 0; j < new_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (i != new_count) {
                strcpy(words[new_count], words[i]);
            }
            new_count++;
        }
    }
    *word_count = new_count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    char *unique_words[MAX_WORDS];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int original_count = word_count;
    remove_duplicates(words, &word_count);
    
    for (int i = 0; i < word_count; i++) {
        unique_words[i] = words[i];
    }
    
    qsort(unique_words, word_count, sizeof(char*), compare_strings);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", original_count);
    printf("Unique words: %d\n\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        int freq = count_word_frequency(unique_words[i], words, original_count);
        printf("%-20s: %d\n", unique_words[i], freq);
    }
    
    return 0;
}