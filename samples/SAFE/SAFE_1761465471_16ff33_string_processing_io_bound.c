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
    char temp_word[MAX_WORD_LEN];
    int word_count = 0;
    
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int pos = 0;
    int in_word = 0;
    int word_pos = 0;
    
    while (buffer[pos] != '\0' && word_count < MAX_WORDS) {
        if (is_valid_char(buffer[pos])) {
            if (!in_word) {
                in_word = 1;
                word_pos = 0;
            }
            if (word_pos < MAX_WORD_LEN - 1) {
                temp_word[word_pos++] = buffer[pos];
            }
        } else {
            if (in_word) {
                temp_word[word_pos] = '\0';
                if (word_pos > 0) {
                    to_lower_case(temp_word);
                    int found_idx = find_word(words, word_count, temp_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else {
                        if (word_count < MAX_WORDS) {
                            strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
                in_word = 0;
            }
        }
        pos++;
    }
    
    if (in_word && word_count < MAX_WORDS) {
        temp_word[word_pos] = '\0';
        if (word_pos > 0) {
            to_lower_case(temp_word);
            int found_idx = find_word(words, word_count, temp_word);
            if (found_idx >= 0) {
                words[found_idx].count++;
            } else {
                strncpy(words[word_count].word, temp_word, MAX_WORD_LEN - 1);
                words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    return word_count;
}

void print_results(struct WordCount *words, int count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
    printf("Total unique words: %d\n", count);
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", MAX_INPUT_LEN - 1);
    printf("> ");
    
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
    
    int word_count = process_input(input, words);
    if (word_count < 0) {
        fprintf(stderr, "Input too long or processing error\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    print_results(words, word_count);
    
    return 0;
}