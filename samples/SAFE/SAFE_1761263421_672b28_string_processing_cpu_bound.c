//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

int extract_words(const char *input, char words[][MAX_INPUT_LEN]) {
    if (!is_valid_input(input)) return -1;
    
    int word_count = 0;
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
                if (word_count < MAX_WORDS && word_len < MAX_INPUT_LEN) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

void process_words(char words[][MAX_INPUT_LEN], int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = (char)toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = (char)tolower((unsigned char)words[i][j]);
            }
        }
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strncpy(temp, words[j], MAX_INPUT_LEN - 1);
                temp[MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_INPUT_LEN - 1);
                words[j][MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_INPUT_LEN - 1);
                words[j + 1][MAX_INPUT_LEN - 1] = '\0';
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    int word_count = extract_words(input, words);
    if (word_count <= 0) {
        fprintf(stderr, "No valid words found\n");
        return EXIT_FAILURE;
    }
    
    process_words(words, word_count);
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return EXIT_SUCCESS;
}