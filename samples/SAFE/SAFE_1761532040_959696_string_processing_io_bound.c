//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int split_into_words(const char *input, char words[][MAX_WORD_LEN]) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && !isspace(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN || word_count >= MAX_WORDS) {
                    return word_count;
                }
                strncpy(words[word_count], input + word_start, word_len);
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
            }
        }
    }
    return word_count;
}

void process_word(char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha(word[i])) {
            if (i % 2 == 0) {
                word[i] = toupper(word[i]);
            } else {
                word[i] = tolower(word[i]);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words);
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        char original[MAX_WORD_LEN];
        strcpy(original, words[i]);
        process_word(words[i]);
        printf("'%s' -> '%s'\n", original, words[i]);
    }
    
    printf("Final result: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}