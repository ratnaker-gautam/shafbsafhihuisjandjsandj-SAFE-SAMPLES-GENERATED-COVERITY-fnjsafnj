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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t len = strlen(text);
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int current_len = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = text[i];
        
        if (is_word_char(c) && current_len < MAX_WORD_LEN - 1) {
            if (!in_word) {
                in_word = 1;
                current_len = 0;
            }
            current_word[current_len++] = tolower((unsigned char)c);
        } else {
            if (in_word) {
                current_word[current_len] = '\0';
                in_word = 0;
                
                if (current_len > 0) {
                    int found = 0;
                    for (int j = 0; j < *word_count; j++) {
                        if (strcmp(words[j].word, current_word) == 0) {
                            words[j].count++;
                            found = 1;
                            break;
                        }
                    }
                    
                    if (!found && *word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                current_len = 0;
            }
        }
    }
    
    return 1;
}

void sort_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && 
                 strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-15s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN] = {0};
    struct WordCount words[MAX_WORDS] = {0};
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: too long or empty.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    print_word_counts(words, word_count);
    
    return 0;
}