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
    for (size_t i = 0; input[i] != '\0'; i++) {
        if (!is_valid_char(input[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, char words[][MAX_WORD_LEN]) {
    if (input == NULL) return 0;
    
    int word_count = 0;
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
                if (word_len < MAX_WORD_LEN && word_count < MAX_WORDS) {
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

int compare_words(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    if (word_count <= 0) return;
    
    qsort(words, word_count, MAX_WORD_LEN, compare_words);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            if (unique_count != i) {
                strcpy(words[unique_count], words[i]);
            }
            unique_count++;
        }
    }
    
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (strlen(words[j]) > strlen(words[j+1])) {
                char temp[MAX_WORD_LEN];
                strcpy(temp, words[j]);
                strcpy(words[j], words[j+1]);
                strcpy(words[j+1], temp);
            }
        }
    }
    
    printf("Processed words (sorted by length):\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s\n", words[i]);
    }
    printf("Total unique words: %d\n", unique_count);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    printf("Enter text to process (alphanumeric characters and whitespace only):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!validate_input(input)) {
        fprintf(stderr, "Invalid input: only alphanumeric characters and whitespace allowed\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found in input\n");
        return 1;
    }
    
    process_words(words, word_count);
    
    return 0;
}