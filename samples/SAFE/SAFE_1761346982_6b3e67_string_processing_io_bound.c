//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

struct WordStats {
    char word[MAX_WORD_LEN];
    int count;
    int length;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_input(const char *input, struct WordStats *words, int *unique_count) {
    if (input == NULL || words == NULL || unique_count == NULL) {
        return -1;
    }
    
    char buffer[MAX_INPUT_LEN];
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    *unique_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?");
    
    while (token != NULL && *unique_count < MAX_WORD_COUNT) {
        size_t token_len = strlen(token);
        if (token_len == 0 || token_len >= MAX_WORD_LEN) {
            token = strtok(NULL, " \t\n\r.,;:!?");
            continue;
        }
        
        int valid_token = 1;
        for (size_t i = 0; i < token_len; i++) {
            if (!is_valid_char(token[i])) {
                valid_token = 0;
                break;
            }
        }
        
        if (!valid_token) {
            token = strtok(NULL, " \t\n\r.,;:!?");
            continue;
        }
        
        for (size_t i = 0; i < token_len; i++) {
            token[i] = tolower((unsigned char)token[i]);
        }
        
        int found = -1;
        for (int i = 0; i < *unique_count; i++) {
            if (strcmp(words[i].word, token) == 0) {
                found = i;
                break;
            }
        }
        
        if (found >= 0) {
            if (words[found].count < INT_MAX) {
                words[found].count++;
            }
        } else {
            strncpy(words[*unique_count].word, token, MAX_WORD_LEN - 1);
            words[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*unique_count].count = 1;
            words[*unique_count].length = (int)token_len;
            (*unique_count)++;
        }
        
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    return 0;
}

void print_stats(const struct WordStats *words, int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word Statistics:\n");
    printf("================\n");
    
    int total_words = 0;
    int longest_word_len = 0;
    
    for (int i = 0; i < count; i++) {
        total_words += words[i].count;
        if (words[i].length > longest_word_len) {
            longest_word_len = words[i].length;
        }
    }
    
    printf("Total unique words: %d\n", count);
    printf("Total word occurrences: %d\n", total_words);
    printf("Longest word length: %d\n\n", longest_word_len);
    
    printf("Word frequency:\n");
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d occurrences\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordStats words[MAX_WORD_COUNT];
    int unique_count = 0;
    
    printf("Enter text for word analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    if (process_input(input, words, &unique_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (unique_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_stats(words, unique_count);
    
    return 0;
}