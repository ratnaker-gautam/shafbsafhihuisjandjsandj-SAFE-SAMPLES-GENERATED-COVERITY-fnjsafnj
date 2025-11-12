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
    if (strlen(input) >= MAX_INPUT_LEN) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lower_case(buffer);
    
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    while (token != NULL && *word_count < MAX_WORDS) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            int valid = 1;
            for (int i = 0; token[i]; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                int index = find_word_index(words, *word_count, token);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    strncpy(words[*word_count].word, token, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    return 1;
}

void print_word_counts(struct WordCount words[], int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}