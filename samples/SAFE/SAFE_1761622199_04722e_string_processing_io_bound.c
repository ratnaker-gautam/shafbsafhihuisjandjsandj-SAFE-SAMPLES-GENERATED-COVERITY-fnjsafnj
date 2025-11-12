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

int add_word(struct WordCount words[], int *word_count, const char *word) {
    if (*word_count >= MAX_WORDS) {
        return 0;
    }
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    
    size_t len = strlen(word);
    if (len >= MAX_WORD_LEN) {
        return 0;
    }
    
    strcpy(words[*word_count].word, word);
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void process_input(const char *input, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) {
        return;
    }
    strcpy(buffer, input);
    
    to_lowercase(buffer);
    
    char current_word[MAX_WORD_LEN];
    int word_pos = 0;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        if (is_valid_char(buffer[i])) {
            if (word_pos < MAX_WORD_LEN - 1) {
                current_word[word_pos++] = buffer[i];
            }
        } else {
            if (word_pos > 0) {
                current_word[word_pos] = '\0';
                add_word(words, word_count, current_word);
                word_pos = 0;
            }
        }
    }
    
    if (word_pos > 0) {
        current_word[word_pos] = '\0';
        add_word(words, word_count, current_word);
    }
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

int main() {
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input provided.\n");
        return 1;
    }
    
    process_input(input, words, &word_count);
    print_results(words, word_count);
    
    return 0;
}