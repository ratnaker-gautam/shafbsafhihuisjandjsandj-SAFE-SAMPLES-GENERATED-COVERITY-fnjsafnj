//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_INPUT_LEN], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    if (!is_valid_input(input, input_len)) return -1;
    
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_INPUT_LEN && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count], input + word_start, word_len);
                    words[*word_count][word_len] = '\0';
                    (*word_count)++;
                }
                in_word = 0;
            }
        }
    }
    
    return 0;
}

void sort_words(char words[][MAX_INPUT_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
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

int count_unique_words(char words[][MAX_INPUT_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) unique_count++;
    }
    return unique_count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = 0;
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input detected\n");
        return EXIT_FAILURE;
    }
    
    if (extract_words(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing words\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return EXIT_SUCCESS;
    }
    
    sort_words(words, word_count);
    int unique_count = count_unique_words(words, word_count);
    
    printf("Sorted words (%d total, %d unique):\n", word_count, unique_count);
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return EXIT_SUCCESS;
}