//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_LEN 64
#define MAX_WORDS 100

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
    
    char temp_word[MAX_WORD_LEN];
    int temp_len = 0;
    *word_count = 0;
    
    for (size_t i = 0; i <= strlen(buffer); i++) {
        char c = buffer[i];
        
        if (is_valid_char(c) && temp_len < MAX_WORD_LEN - 1) {
            temp_word[temp_len++] = tolower((unsigned char)c);
        } else {
            if (temp_len > 0) {
                temp_word[temp_len] = '\0';
                
                int found = 0;
                for (int j = 0; j < *word_count; j++) {
                    if (strcmp(words[j].word, temp_word) == 0) {
                        words[j].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found && *word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                
                temp_len = 0;
            }
            
            if (c == '\0') {
                break;
            }
        }
    }
    
    return 0;
}

void print_results(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_results(words, word_count);
    
    return 0;
}