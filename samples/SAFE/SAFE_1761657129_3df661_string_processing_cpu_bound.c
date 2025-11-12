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

int extract_words(const char *input, char words[][32]) {
    if (input == NULL || words == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return 0;
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lowercase(buffer);
    
    int word_count = 0;
    const char *delimiters = " \t\n\r\f\v.,;:!?\"'()[]{}";
    char *token = strtok(buffer, delimiters);
    
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) < 31) {
            strncpy(words[word_count], token, 31);
            words[word_count][31] = '\0';
            word_count++;
        }
        token = strtok(NULL, delimiters);
    }
    
    return word_count;
}

void count_word_frequencies(char words[][32], int word_count, char unique_words[][32], int frequencies[], int *unique_count) {
    if (words == NULL || unique_words == NULL || frequencies == NULL || unique_count == NULL) return;
    
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
            strncpy(unique_words[*unique_count], words[i], 31);
            unique_words[*unique_count][31] = '\0';
            frequencies[*unique_count] = 1;
            (*unique_count)++;
        }
    }
}

void sort_by_frequency(char unique_words[][32], int frequencies[], int unique_count) {
    if (unique_words == NULL || frequencies == NULL) return;
    
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (frequencies[j] < frequencies[j + 1] || 
                (frequencies[j] == frequencies[j + 1] && strcmp(unique_words[j], unique_words[j + 1]) > 0)) {
                int temp_freq = frequencies[j];
                frequencies[j] = frequencies[j + 1];
                frequencies[j + 1] = temp_freq;
                
                char temp_word[32];
                strncpy(temp_word, unique_words[j], 31);
                temp_word[31] = '\0';
                strncpy(unique_words[j], unique_words[j + 1], 31);
                unique_words[j][31] = '\0';
                strncpy(unique_words[j + 1], temp_word, 31);
                unique_words[j + 1][31] = '\0';
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    char unique_words[MAX_WORDS][32];
    int frequencies[MAX_WORDS];
    
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
    
    int word_count = extract_words(input, words);
    if (word_count <= 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    int unique_count = 0;
    count_word_frequencies(words, word_count, unique_words, frequencies, &unique_count);
    
    sort_by_frequency(unique_words, frequencies, unique_count);
    
    printf("\nWord frequencies (sorted by frequency then alphabetically):\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    return 0;
}