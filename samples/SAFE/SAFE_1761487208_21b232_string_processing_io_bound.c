//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) {
            return 0;
        }
    }
    return 1;
}

int split_into_words(const char *str, char words[][32], size_t max_words) {
    if (str == NULL || max_words == 0) {
        return 0;
    }
    
    int word_count = 0;
    size_t i = 0;
    
    while (str[i] != '\0' && word_count < max_words) {
        while (str[i] != '\0' && isspace(str[i])) {
            i++;
        }
        
        if (str[i] == '\0') {
            break;
        }
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (str[i] != '\0' && !isspace(str[i]) && word_len < 31) {
            words[word_count][word_len] = str[i];
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void process_word(char *word) {
    if (word == NULL || word[0] == '\0') {
        return;
    }
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha(word[i])) {
            if (i % 2 == 0) {
                word[i] = toupper(word[i]);
            } else {
                word[i] = tolower(word[i]);
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    
    printf("Enter text to process (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Original words: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Processed words: ");
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
        printf("%s ", words[i]);
    }
    printf("\n");
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}