//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    char temp_word[MAX_WORD_LEN];
    int buf_len = strlen(input);
    
    if (buf_len >= MAX_INPUT_LEN - 1) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lower_case(buffer);
    
    int pos = 0;
    int word_start = -1;
    int current_word_len = 0;
    
    while (pos <= buf_len) {
        char c = buffer[pos];
        
        if (is_valid_char(c) && current_word_len < MAX_WORD_LEN - 1) {
            if (word_start == -1) {
                word_start = pos;
            }
            temp_word[current_word_len++] = c;
        } else {
            if (current_word_len > 0) {
                temp_word[current_word_len] = '\0';
                
                if (current_word_len >= 2) {
                    int idx = find_word_index(words, *word_count, temp_word);
                    if (idx >= 0) {
                        words[idx].count++;
                    } else if (*word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    } else {
                        return -2;
                    }
                }
                
                current_word_len = 0;
                word_start = -1;
            }
        }
        pos++;
    }
    
    return 0;
}

void print_word_counts(struct WordCount words[], int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
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
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 0;
    }
    
    int result = process_input(input, words, &word_count);
    
    if (result == -1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    } else if (result == -2) {
        fprintf(stderr, "Too many unique words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}