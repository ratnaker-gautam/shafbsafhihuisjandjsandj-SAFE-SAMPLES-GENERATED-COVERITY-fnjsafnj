//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void trim_trailing_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int split_into_words(const char *text, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t i = 0;
    
    while (text[i] != '\0' && word_count < max_words) {
        while (text[i] != '\0' && isspace(text[i])) i++;
        if (text[i] == '\0') break;
        
        size_t j = 0;
        while (text[i] != '\0' && !isspace(text[i]) && j < 31) {
            words[word_count][j] = text[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        word_count++;
    }
    
    return word_count;
}

void process_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (j % 2 == 0) {
                words[i][j] = toupper(words[i][j]);
            } else {
                words[i][j] = tolower(words[i][j]);
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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        return 1;
    }
    
    trim_trailing_newline(input);
    size_t input_len = strlen(input);
    
    if (!is_valid_input(input, input_len)) {
        if (printf("Invalid input\n") < 0) return 1;
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        if (printf("No words found\n") < 0) return 1;
        return 1;
    }
    
    process_words(words, word_count);
    print_words(words, word_count);
    
    return 0;
}