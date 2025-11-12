//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500
#define MAX_WORD_LEN 50

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, char words[][MAX_WORD_LEN]) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return -1;
    strcpy(buffer, input);
    to_lowercase(buffer);
    
    int word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && word_count < MAX_WORDS) {
        while (buffer[i] != '\0' && !isalnum((unsigned char)buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && isalnum((unsigned char)buffer[i])) {
            i++;
        }
        
        size_t length = i - start;
        if (length >= MAX_WORD_LEN) continue;
        
        strncpy(words[word_count], &buffer[start], length);
        words[word_count][length] = '\0';
        word_count++;
    }
    
    return word_count;
}

int compare_words(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void count_word_frequencies(char words[][MAX_WORD_LEN], int word_count, 
                           char unique_words[][MAX_WORD_LEN], int frequencies[], int *unique_count) {
    if (word_count <= 0) {
        *unique_count = 0;
        return;
    }
    
    char sorted_words[MAX_WORDS][MAX_WORD_LEN];
    for (int i = 0; i < word_count; i++) {
        strcpy(sorted_words[i], words[i]);
    }
    
    qsort(sorted_words, word_count, MAX_WORD_LEN, compare_words);
    
    *unique_count = 0;
    int count = 1;
    
    for (int i = 1; i <= word_count; i++) {
        if (i < word_count && strcmp(sorted_words[i], sorted_words[i-1]) == 0) {
            count++;
        } else {
            if (*unique_count < MAX_WORDS) {
                strcpy(unique_words[*unique_count], sorted_words[i-1]);
                frequencies[*unique_count] = count;
                (*unique_count)++;
            }
            count = 1;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    char unique_words[MAX_WORDS][MAX_WORD_LEN];
    int frequencies[MAX_WORDS];
    
    printf("Enter text to analyze (alphanumeric characters and whitespace only):\n");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and whitespace allowed\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_count;
    count_word_frequencies(words, word_count, unique_words, frequencies, &unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    return 0;
}