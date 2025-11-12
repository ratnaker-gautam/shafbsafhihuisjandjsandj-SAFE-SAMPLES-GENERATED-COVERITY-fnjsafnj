//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[MAX_WORDS][MAX_WORD_LEN]) {
    if (input == NULL) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_pos = 0;
    
    for (size_t i = 0; i <= input_len && word_count < MAX_WORDS; i++) {
        if (i < input_len && isalnum((unsigned char)input[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                words[word_count][word_pos++] = input[i];
            }
        } else {
            if (word_pos > 0) {
                words[word_count][word_pos] = '\0';
                word_count++;
                word_pos = 0;
            }
        }
    }
    
    return word_count;
}

void sort_words(char words[MAX_WORDS][MAX_WORD_LEN], int word_count) {
    if (word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], MAX_WORD_LEN - 1);
                temp[MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_WORD_LEN - 1);
                words[j + 1][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
}

int count_unique_words(char words[MAX_WORDS][MAX_WORD_LEN], int word_count) {
    if (word_count == 0) return 0;
    
    int unique_count = 1;
    for (int i = 1; i < word_count; i++) {
        if (strcmp(words[i], words[i - 1]) != 0) {
            unique_count++;
        }
    }
    return unique_count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    int word_count = extract_words(input, words);
    if (word_count == 0) {
        printf("No words found in input\n");
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