//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int *word_count) {
    if (!validate_input(input)) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN || *word_count >= MAX_WORDS) {
                    return 0;
                }
                if (word_len > 0) {
                    strncpy(words[*word_count], &input[word_start], word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_word_frequency(const char words[][MAX_WORD_LEN], int word_count, const char *target) {
    if (target == NULL || strlen(target) >= MAX_WORD_LEN) return 0;
    
    int count = 0;
    char lower_target[MAX_WORD_LEN];
    strncpy(lower_target, target, MAX_WORD_LEN - 1);
    lower_target[MAX_WORD_LEN - 1] = '\0';
    to_lowercase(lower_target);
    
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN];
        strncpy(temp, words[i], MAX_WORD_LEN - 1);
        temp[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp);
        if (strcmp(temp, lower_target) == 0) {
            count++;
        }
    }
    return count;
}

void print_word_stats(const char words[][MAX_WORD_LEN], int word_count) {
    if (word_count <= 0) return;
    
    printf("Word Statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            char temp1[MAX_WORD_LEN], temp2[MAX_WORD_LEN];
            strncpy(temp1, words[i], MAX_WORD_LEN - 1);
            strncpy(temp2, words[j], MAX_WORD_LEN - 1);
            temp1[MAX_WORD_LEN - 1] = '\0';
            temp2[MAX_WORD_LEN - 1] = '\0';
            to_lowercase(temp1);
            to_lowercase(temp2);
            if (strcmp(temp1, temp2) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) unique_count++;
    }
    printf("Unique words: %d\n", unique_count);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
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
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    print_word_stats(words, word_count);
    
    char search_word[MAX_WORD_LEN];
    printf("\nEnter a word to search for: ");
    if (fgets(search_word, sizeof(search_word), stdin) == NULL) {
        fprintf(stderr, "Error reading search word\n");
        return 1;
    }
    
    len = strlen(search_word);
    if (len > 0 && search_word[len - 1] == '\n') {
        search_word[len - 1] = '\0';