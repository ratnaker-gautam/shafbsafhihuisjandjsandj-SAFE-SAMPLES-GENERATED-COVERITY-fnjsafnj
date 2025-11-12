//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_input(const char *input, struct WordCount words[], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return -1;
    }
    
    char buffer[MAX_INPUT_LEN];
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    *word_count = 0;
    char *ptr = buffer;
    
    while (*ptr != '\0' && *word_count < MAX_WORDS) {
        while (*ptr != '\0' && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr != '\0' && is_valid_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, word_start, word_len);
        current_word[word_len] = '\0';
        
        for (size_t i = 0; i < word_len; i++) {
            current_word[i] = tolower((unsigned char)current_word[i]);
        }
        
        int found = 0;
        for (int i = 0; i < *word_count; i++) {
            if (strcmp(words[i].word, current_word) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 0;
}

void print_results(const struct WordCount words[], int word_count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty input provided.\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input.\n");
        return 1;
    }
    
    print_results(words, word_count);
    
    return 0;
}