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
    if (input == NULL) return 0;
    int word_count = 0;
    size_t i = 0;
    while (input[i] != '\0' && word_count < MAX_WORDS) {
        while (input[i] != '\0' && isspace((unsigned char)input[i])) i++;
        if (input[i] == '\0') break;
        size_t start = i;
        while (input[i] != '\0' && !isspace((unsigned char)input[i])) i++;
        size_t word_len = i - start;
        if (word_len >= MAX_INPUT_LEN) return -1;
        for (size_t j = 0; j < word_len; j++) {
            words[word_count][j] = input[start + j];
        }
        words[word_count][word_len] = '\0';
        word_count++;
    }
    return word_count;
}

void sort_words(char words[][MAX_INPUT_LEN], int count) {
    if (words == NULL || count <= 0) return;
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_INPUT_LEN];
                strncpy(temp, words[j], MAX_INPUT_LEN);
                temp[MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_INPUT_LEN);
                words[j][MAX_INPUT_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_INPUT_LEN);
                words[j + 1][MAX_INPUT_LEN - 1] = '\0';
            }
        }
    }
}

int count_unique_words(char words[][MAX_INPUT_LEN], int count) {
    if (words == NULL || count <= 0) return 0;
    int unique = 0;
    for (int i = 0; i < count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) unique++;
    }
    return unique;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter text: ");
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
    char words[MAX_WORDS][MAX_INPUT_LEN];
    int word_count = extract_words(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Input processing error\n");
        return 1;
    }
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    sort_words(words, word_count);
    int unique_count = count_unique_words(words, word_count);
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    printf("Sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    return 0;
}