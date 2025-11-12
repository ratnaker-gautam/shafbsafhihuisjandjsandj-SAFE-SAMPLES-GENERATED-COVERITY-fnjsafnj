//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int split_words(const char *input, char words[][32], int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    size_t len = strlen(input);
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && !isspace(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= 32) return 0;
                if (count >= MAX_WORDS) return 0;
                
                strncpy(words[count], &input[word_start], word_len);
                words[count][word_len] = '\0';
                count++;
                in_word = 0;
            }
        }
    }
    
    *word_count = count;
    return 1;
}

void process_word(char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (i % 2 == 0) {
            word[i] = toupper(word[i]);
        } else {
            word[i] = tolower(word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    int word_count = 0;
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (!split_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("Original words: %d\n", word_count);
    printf("Processing words...\n");
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
        printf("Word %d: %s\n", i + 1, words[i]);
    }
    
    printf("Processing complete.\n");
    return 0;
}