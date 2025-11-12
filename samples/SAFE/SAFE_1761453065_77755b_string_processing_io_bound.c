//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_words(const char *input, char words[][32], int *word_count) {
    if (!is_valid_input(input)) return 0;
    *word_count = 0;
    size_t len = strlen(input);
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && !isspace((unsigned char)input[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= 32 || *word_count >= MAX_WORDS) return 0;
                strncpy(words[*word_count], input + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void process_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (j % 2 == 0) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            } else {
                words[i][j] = tolower((unsigned char)words[i][j]);
            }
        }
    }
}

void print_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (printf("%s", words[i]) < 0) exit(1);
        if (i < word_count - 1) {
            if (printf(" ") < 0) exit(1);
        }
    }
    if (printf("\n") < 0) exit(1);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    int word_count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        return 1;
    }
    
    if (!split_words(input, words, &word_count)) {
        return 1;
    }
    
    if (word_count == 0) {
        return 1;
    }
    
    process_words(words, word_count);
    print_words(words, word_count);
    
    return 0;
}