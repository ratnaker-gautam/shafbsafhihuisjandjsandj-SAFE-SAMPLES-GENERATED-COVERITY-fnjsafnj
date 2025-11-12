//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int validate_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    return 1;
}

int count_words(const char* input, char words[][MAX_WORD_LEN]) {
    if (!validate_input(input)) return 0;
    
    int word_count = 0;
    int in_word = 0;
    size_t word_len = 0;
    
    for (size_t i = 0; i <= strlen(input); i++) {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                words[word_count][word_len++] = input[i];
            }
        } else {
            if (in_word) {
                words[word_count][word_len] = '\0';
                word_count++;
                in_word = 0;
                if (word_count >= MAX_WORDS) break;
            }
        }
    }
    
    return word_count;
}

void reverse_string(char* str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

void process_words(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    for (int i = 0; i < word_count; i++) {
        if (i % 2 == 0) {
            for (size_t j = 0; j < strlen(words[i]); j++) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            }
        } else {
            reverse_string(words[i]);
        }
    }
}

void display_results(char words[][MAX_WORD_LEN], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nWord count: %d\n", word_count);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!validate_input(input)) {
        printf("Invalid input: empty or too long.\n");
        return 1;
    }
    
    int word_count = count_words(input, words);
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 1;
    }
    
    process_words(words, word_count);
    display_results(words, word_count);
    
    return 0;
}