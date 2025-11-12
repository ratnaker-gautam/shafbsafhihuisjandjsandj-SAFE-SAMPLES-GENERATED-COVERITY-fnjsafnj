//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

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

int find_word(struct WordCount *words, int count, const char *word) {
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

int process_input(const char *input, struct WordCount *words) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) {
        return -1;
    }
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lower_case(buffer);
    
    int word_count = 0;
    char *token = buffer;
    char *end = buffer;
    
    while (*token && word_count < MAX_WORDS) {
        while (*token && !is_valid_char(*token)) {
            token++;
        }
        if (!*token) break;
        
        end = token;
        while (*end && is_valid_char(*end)) {
            end++;
        }
        
        if (end - token >= MAX_WORD_LEN) {
            token = end;
            continue;
        }
        
        char current_word[MAX_WORD_LEN];
        strncpy(current_word, token, end - token);
        current_word[end - token] = '\0';
        
        int existing_index = find_word(words, word_count, current_word);
        if (existing_index >= 0) {
            if (words[existing_index].count < INT_MAX) {
                words[existing_index].count++;
            }
        } else {
            if (strlen(current_word) > 0) {
                strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        token = end;
    }
    
    return word_count;
}

void print_results(struct WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
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
        return 0;
    }
    
    int word_count = process_input(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Input too long or processing error\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    print_results(words, word_count);
    
    return 0;
}