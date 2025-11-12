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
    
    char temp_word[MAX_WORD_LEN];
    int temp_index = 0;
    int in_word = 0;
    
    for (int i = 0; buffer[i]; i++) {
        if (is_valid_char(buffer[i])) {
            if (temp_index < MAX_WORD_LEN - 1) {
                temp_word[temp_index++] = buffer[i];
                in_word = 1;
            }
        } else {
            if (in_word && temp_index > 0) {
                temp_word[temp_index] = '\0';
                
                int existing_index = find_word_index(words, *word_count, temp_word);
                if (existing_index >= 0) {
                    words[existing_index].count++;
                } else if (*word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
                
                temp_index = 0;
                in_word = 0;
            }
        }
    }
    
    if (in_word && temp_index > 0 && *word_count < MAX_WORDS) {
        temp_word[temp_index] = '\0';
        int existing_index = find_word_index(words, *word_count, temp_word);
        if (existing_index >= 0) {
            words[existing_index].count++;
        } else {
            strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
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
        printf("No input provided.\n");
        return 1;
    }
    
    if (!process_input(input, words, &word_count)) {
        printf("Input too long or processing error.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_word_counts(words, word_count);
    
    return 0;
}