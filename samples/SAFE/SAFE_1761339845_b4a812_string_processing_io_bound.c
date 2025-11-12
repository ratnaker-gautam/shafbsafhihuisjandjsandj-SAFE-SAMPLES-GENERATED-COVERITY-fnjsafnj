//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) {
            return 0;
        }
    }
    return 1;
}

void trim_whitespace(char *str) {
    if (str == NULL) return;
    
    char *start = str;
    while (isspace((unsigned char)*start)) start++;
    
    if (*start == '\0') {
        str[0] = '\0';
        return;
    }
    
    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    
    *(end + 1) = '\0';
    
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}

int split_into_words(const char *str, char words[][32], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char *p = str;
    
    while (*p && word_count < max_words) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (*p == '\0') break;
        
        const char *word_start = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        
        size_t word_len = p - word_start;
        if (word_len >= 32) word_len = 31;
        
        if (word_len > 0) {
            memcpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void process_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        char *word = words[i];
        size_t len = strlen(word);
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)word[j])) {
                if (islower((unsigned char)word[j])) {
                    word[j] = (char)toupper((unsigned char)word[j]);
                } else {
                    word[j] = (char)tolower((unsigned char)word[j]);
                }
            }
        }
    }
}

void print_words(char words[][32], int word_count) {
    for (int i = 0; i < word_count; i++) {
        if (i > 0) printf(" ");
        printf("%s", words[i]);
    }
    printf("\n");
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
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    process_words(words, word_count);
    
    printf("Processed text: ");
    print_words(words, word_count);
    
    return 0;
}