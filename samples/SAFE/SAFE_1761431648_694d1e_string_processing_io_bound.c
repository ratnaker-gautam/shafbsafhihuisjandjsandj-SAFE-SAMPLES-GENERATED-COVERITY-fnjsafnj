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

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    if (len > 0) {
        for (size_t i = 0; i < len; i++) {
            if (isalpha((unsigned char)word[i])) {
                word[i] = toupper((unsigned char)word[i]);
            }
        }
    }
}

int split_into_words(const char *input, char words[][64], int max_words) {
    if (input == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char *start = input;
    const char *end = input;
    
    while (*start && word_count < max_words) {
        while (*start && !isalpha((unsigned char)*start)) {
            start++;
        }
        if (*start == '\0') break;
        
        end = start;
        while (*end && isalpha((unsigned char)*end)) {
            end++;
        }
        
        size_t word_len = end - start;
        if (word_len > 0 && word_len < 64) {
            strncpy(words[word_count], start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        start = end;
    }
    
    return word_count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
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
    
    printf("Processing %d words...\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    return 0;
}