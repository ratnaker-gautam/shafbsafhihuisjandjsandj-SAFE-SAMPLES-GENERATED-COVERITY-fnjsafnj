//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t max_len) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > max_len) return 0;
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

int extract_words(const char *input, char words[][32], size_t max_words) {
    if (input == NULL || max_words == 0) return 0;
    
    size_t word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 32) {
                    strncpy(words[word_count], &input[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    to_lowercase(words[word_count]);
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void process_string_analysis(const char *input) {
    if (!is_valid_input(input, MAX_INPUT_LEN)) {
        printf("Invalid input\n");
        return;
    }
    
    char words[MAX_WORDS][32] = {0};
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return;
    }
    
    qsort(words, word_count, 32, compare_strings);
    
    printf("Sorted unique words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    char longest_word[32] = "";
    char shortest_word[32] = "";
    size_t shortest_len = SIZE_MAX;
    size_t longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > longest_len) {
            longest_len = len;
            strncpy(longest_word, words[i], 31);
            longest_word[31] = '\0';
        }
        if (len < shortest_len && len > 0) {
            shortest_len = len;
            strncpy(shortest_word, words[i], 31);
            shortest_word[31] = '\0';
        }
    }
    
    printf("\nLongest word: %s\n", longest_word);
    printf("Shortest word: %s\n", shortest_word);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Total unique words: %d\n", unique_count);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for string analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    process_string_analysis(input);
    
    return 0;
}