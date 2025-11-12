//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower(str[i]);
    }
}

int extract_words(const char *input, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t word_len = 0;
    char current_word[32] = {0};
    
    for (size_t i = 0; input[i] != '\0' && word_count < max_words; i++) {
        if (isalpha(input[i])) {
            if (word_len < 31) {
                current_word[word_len++] = input[i];
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                strncpy(words[word_count], current_word, 31);
                words[word_count][31] = '\0';
                word_count++;
                word_len = 0;
            }
        }
    }
    
    if (word_len > 0 && word_count < max_words) {
        current_word[word_len] = '\0';
        strncpy(words[word_count], current_word, 31);
        words[word_count][31] = '\0';
        word_count++;
    }
    
    return word_count;
}

void count_word_frequencies(char words[][32], int word_count, 
                           char unique_words[][32], int frequencies[], int *unique_count) {
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
    char input[MAX_INPUT_LEN] = {0};
    char words[MAX_WORDS][32] = {{0}};
    char unique_words[MAX_WORDS][32] = {{0}};
    int frequencies[MAX_WORDS] = {0};
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    to_lowercase(input, input_len);
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_count = 0;
    count_word_frequencies(words, word_count, unique_words, frequencies, &unique_count);
    
    sort_by_frequency(unique_words, frequencies, unique_count);
    
    printf("\nWord frequencies (sorted by frequency, then alphabetically):\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    return 0;
}