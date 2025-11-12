//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void trim_whitespace(char *str) {
    size_t len = strlen(str);
    size_t start = 0;
    size_t end = len - 1;
    
    while (start < len && isspace((unsigned char)str[start])) start++;
    while (end > start && isspace((unsigned char)str[end])) end--;
    
    if (start > 0 || end < len - 1) {
        memmove(str, str + start, end - start + 1);
        str[end - start + 1] = '\0';
    }
}

int split_words(const char *str, char words[][32], size_t max_words) {
    size_t word_count = 0;
    size_t i = 0;
    size_t len = strlen(str);
    
    while (i < len && word_count < max_words) {
        while (i < len && isspace((unsigned char)str[i])) i++;
        if (i >= len) break;
        
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)str[i])) i++;
        size_t word_len = i - word_start;
        
        if (word_len >= 32) word_len = 31;
        memcpy(words[word_count], str + word_start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return (int)word_count;
}

void process_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        for (size_t j = 0; j < len; j++) {
            if (islower((unsigned char)words[i][j])) {
                words[i][j] = (char)toupper((unsigned char)words[i][j]);
            } else if (isupper((unsigned char)words[i][j])) {
                words[i][j] = (char)tolower((unsigned char)words[i][j]);
            }
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
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input detected\n");
        return 1;
    }
    
    trim_whitespace(input);
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Input is empty after trimming\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        fprintf(stderr, "No words found in input\n");
        return 1;
    }
    
    process_words(words, word_count);
    
    printf("Processed text: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) printf(" ");
    }
    printf("\n");
    
    return 0;
}