//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int current_len = 0;
    
    while (input[pos] != '\0' && word_count < max_words) {
        if (is_valid_char(input[pos])) {
            if (!in_word) {
                in_word = 1;
                current_len = 0;
            }
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = tolower((unsigned char)input[pos]);
            }
        } else {
            if (in_word) {
                current_word[current_len] = '\0';
                if (current_len > 0) {
                    strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_count++;
                }
                in_word = 0;
            }
        }
        pos++;
    }
    
    if (in_word && current_len > 0 && word_count < max_words) {
        current_word[current_len] = '\0';
        strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
    }
    
    return word_count;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *unique_words) {
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *unique_words, int unique_count) {
    for (int i = 0; i < unique_count - 1; i++) {
        for (int j = 0; j < unique_count - i - 1; j++) {
            if (unique_words[j].count < unique_words[j + 1].count) {
                struct WordCount temp = unique_words[j];
                unique_words[j] = unique_words[j + 1];
                unique_words[j + 1] = temp;
            }
        }
    }
}

int get_user_input(char *buffer, int max_len) {
    printf("Enter text to analyze (max %d characters): ", max_len - 1);
    
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && buffer[0] == '\n')) {
        return 0;
    }
    
    return 1;
}

void display_results(struct WordCount *unique_words, int unique_count) {
    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-15s: %d\n", unique_words[i].word, unique_words[i].count);
    }
    
    if (unique_count > 10) {
        printf("... and %d more words\n", unique_count - 10);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_words[MAX_WORDS];
    
    if (!get_user_input(input, sizeof(input))) {
        printf("No input provided or input error.\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("Empty input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    sort_word_counts(unique_words, unique_count);
    display_results(unique_words, unique_count);
    
    return 0;
}