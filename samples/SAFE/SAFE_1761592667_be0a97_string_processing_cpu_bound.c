//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

int validate_input(const char *input) {
    if (input == NULL) return 0;
    for (size_t i = 0; i < strlen(input); i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

void extract_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalnum((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < MAX_WORD_LEN && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count], input + word_start, word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
                in_word = 0;
            }
        }
    }
}

void sort_words(char words[][MAX_WORD_LEN], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j + 1]);
                strcpy(words[j + 1], temp);
            }
        }
    }
}

void count_word_frequency(char words[][MAX_WORD_LEN], int word_count, 
                         char unique_words[][MAX_WORD_LEN], int *unique_count, 
                         int frequencies[]) {
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        if (!found && *unique_count < MAX_WORDS) {
            strcpy(unique_words[*unique_count], words[i]);
            frequencies[*unique_count] = 1;
            (*unique_count)++;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    char unique_words[MAX_WORDS][MAX_WORD_LEN];
    int frequencies[MAX_WORDS];
    int word_count, unique_count;
    
    printf("Enter text (alphanumeric characters and whitespace only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: contains invalid characters\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Error: Empty input\n");
        return 1;
    }
    
    extract_words(input, words, &word_count);
    
    if (word_count == 0) {
        fprintf(stderr, "Error: No valid words found\n");
        return 1;
    }
    
    sort_words(words, word_count);
    count_word_frequency(words, word_count, unique_words, &unique_count, frequencies);
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    for (int i = 0; i < unique_count; i++) {
        printf("%-20s: %d\n", unique_words[i], frequencies[i]);
    }
    
    return 0;
}