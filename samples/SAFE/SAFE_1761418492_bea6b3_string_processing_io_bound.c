//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int split_into_words(const char *text, char words[][32], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    int word_count = 0;
    size_t text_len = strlen(text);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len && word_count < max_words; i++) {
        if (i < text_len && isalpha((unsigned char)text[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 32) {
                    strncpy(words[word_count], &text[word_start], word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
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
        if (i % 2 == 0) {
            word[i] = toupper((unsigned char)word[i]);
        } else {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process: ");
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
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
        printf("Word %d: %s\n", i + 1, words[i]);
    }
    
    printf("Original input: %s\n", input);
    printf("Processing complete\n");
    
    return 0;
}