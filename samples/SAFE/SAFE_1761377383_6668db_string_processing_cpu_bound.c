//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1000
#define MAX_WORDS 500

struct WordCount {
    char word[50];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_word(char *word, struct WordCount *words, int *word_count) {
    if (strlen(word) == 0) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            if (words[i].count < INT_MAX) {
                words[i].count++;
            }
            return 0;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        strncpy(words[*word_count].word, word, sizeof(words[*word_count].word) - 1);
        words[*word_count].word[sizeof(words[*word_count].word) - 1] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
        return 1;
    }
    
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN + 1];
    char current_word[50];
    int buf_len = strlen(input);
    
    if (buf_len > MAX_INPUT_LEN) {
        return -1;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    int word_pos = 0;
    int in_word = 0;
    
    for (int i = 0; i <= buf_len; i++) {
        char c = buffer[i];
        
        if (is_valid_char(c)) {
            if (word_pos < 49) {
                current_word[word_pos++] = c;
            }
            in_word = 1;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                to_lower_case(current_word);
                if (process_word(current_word, words, word_count) == -1) {
                    return -1;
                }
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    return 0;
}

void sort_words(struct WordCount *words, int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (extract_words(input, words, &word_count) == -1) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis (sorted by frequency, then alphabetically):\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}