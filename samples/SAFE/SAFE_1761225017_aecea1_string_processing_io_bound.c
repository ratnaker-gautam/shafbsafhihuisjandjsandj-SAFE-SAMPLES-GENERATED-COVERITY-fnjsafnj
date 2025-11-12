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

int split_words(const char *text, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t i = 0;
    size_t text_len = strlen(text);
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && isspace(text[i])) i++;
        if (i >= text_len) break;
        
        size_t word_start = i;
        while (i < text_len && !isspace(text[i])) i++;
        
        size_t word_len = i - word_start;
        if (word_len >= 32) word_len = 31;
        
        strncpy(words[word_count], &text[word_start], word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return (int)word_count;
}

void process_words(char words[][32], int count) {
    for (int i = 0; i < count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (islower(words[i][j])) {
                words[i][j] = (char)toupper(words[i][j]);
            } else if (isupper(words[i][j])) {
                words[i][j] = (char)tolower(words[i][j]);
            }
        }
    }
}

void print_words(char words[][32], int count) {
    for (int i = 0; i < count; i++) {
        if (printf("%s", words[i]) < 0) exit(1);
        if (i < count - 1) {
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        if (printf("Invalid input\n") < 0) return 1;
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        if (printf("No words found\n") < 0) return 1;
        return 1;
    }
    
    process_words(words, word_count);
    print_words(words, word_count);
    
    return 0;
}