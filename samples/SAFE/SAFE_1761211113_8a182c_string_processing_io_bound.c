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

int split_words(const char *str, char words[][32], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(str);
    
    while (i < len && word_count < max_words) {
        while (i < len && isspace((unsigned char)str[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && !isspace((unsigned char)str[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= 32) word_len = 31;
        
        if (word_len > 0) {
            strncpy(words[word_count], &str[start], word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void process_word(char *word) {
    if (word == NULL || strlen(word) == 0) return;
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            if (i % 2 == 0) {
                word[i] = toupper((unsigned char)word[i]);
            } else {
                word[i] = tolower((unsigned char)word[i]);
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    return 0;
}