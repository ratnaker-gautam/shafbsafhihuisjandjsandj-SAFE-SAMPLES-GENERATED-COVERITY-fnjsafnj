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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    int word_count = 0;
    int len = strlen(input);
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    
    for (int i = 0; i <= len; i++) {
        if (i < len && is_word_char(input[i])) {
            if (buf_pos < MAX_WORD_LEN - 1) {
                buffer[buf_pos++] = input[i];
            }
        } else {
            if (buf_pos > 0) {
                buffer[buf_pos] = '\0';
                to_lower_case(buffer);
                
                int existing = find_word(words, word_count, buffer);
                if (existing >= 0) {
                    words[existing].count++;
                } else if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, buffer, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
                buf_pos = 0;
            }
        }
    }
    
    return word_count;
}

void print_results(struct WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 0;
    }
    
    int word_count = process_input(input, words);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    print_results(words, word_count);
    
    return 0;
}