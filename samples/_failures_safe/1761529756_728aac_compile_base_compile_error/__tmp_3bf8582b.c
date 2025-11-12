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
    size_t len = strlen(input);
    for (size_t i = 0; i < len; i++) {
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
                strncpy(temp, words[j], MAX_WORD_LEN);
                strncpy(words[j], words[j + 1], MAX_WORD_LEN);
                strncpy(words[j + 1], temp, MAX_WORD_LEN);
            }
        }
    }
}

void process_word_frequencies(char words[][MAX_WORD_LEN], int word_count) {
    if (word_count == 0) return;
    
    char unique_words[MAX_WORDS][MAX_WORD_LEN];
    int frequencies[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count], words[i], MAX_WORD_LEN);
            frequencies[unique_count] = 1;
            unique_count++;
        }
    }
    
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (frequencies[j] < frequencies[j + 1]) {
                int temp_freq = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp_freq;
                
                char temp_word[MAX_WORD_LEN];
                strncpy(temp_word, unique_words[j], MAX_WORD_LEN);
                strncpy(unique_words[j], unique_words[j + 1], MAX_WORD_LEN);
                strncpy(unique_words[j + 1], temp_word, MAX_WORD_LEN);
            }
        }
    }
    
    printf("Word frequencies (sorted by frequency):\n");
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and whitespace allowed\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    extract_words(input, words, &word_count);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("Sorted words:\n");
    for (int i = 0; i < word_count && i