//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

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

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_text(const char *text, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN - 1) {
        return;
    }
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    while (token != NULL && *word_count < MAX_WORDS) {
        int len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            int valid = 1;
            for (int i = 0; i < len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                to_lower_case(token);
                int index = find_word_index(words, *word_count, token);
                if (index >= 0) {
                    if (words[index].count < INT_MAX) {
                        words[index].count++;
                    }
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
}

void print_word_counts(struct WordCount *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (empty line to finish):\n");
    
    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        if (input[0] == '\n' || input[0] == '\0') {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        process_text(input, words, &word_count);
        
        if (word_count >= MAX_WORDS) {
            printf("Maximum word count reached.\n");
            break;
        }
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No valid words found.\n");
    }
    
    return 0;
}