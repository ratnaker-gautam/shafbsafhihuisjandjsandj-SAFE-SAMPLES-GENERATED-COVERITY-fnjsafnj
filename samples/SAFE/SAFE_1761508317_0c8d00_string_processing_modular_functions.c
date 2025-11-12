//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN) {
        return 0;
    }
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"()[]{}");
    
    while (token != NULL && word_count < max_words) {
        if (strlen(token) > 0 && strlen(token) < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < strlen(token); i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                for (size_t i = 0; i < strlen(token); i++) {
                    words[word_count].word[i] = tolower((unsigned char)token[i]);
                }
                words[word_count].word[strlen(token)] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"()[]{}");
    }
    
    return word_count;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                word_count--;
                j--;
            }
        }
    }
    
    return word_count;
}

void sort_words_by_count(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) {
        return;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_stats(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count; i++) {
        if (i < 10) {
            printf("%2d. %-20s: %d\n", i + 1, words[i].word, words[i].count);
        }
    }
    
    if (word_count > 10) {
        printf("... and %d more words\n", word_count - 10);
    }
}

int main(void) {
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
    
    int initial_count = extract_words(input, words, MAX_WORDS);
    if (initial_count == 0) {
        printf("No valid words found in the input.\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, initial_count);
    if (unique_count == 0) {
        printf("Error processing words.\n");
        return 1;
    }
    
    sort_words_by_count(words, unique_count);
    print_word_stats(words, unique_count);
    
    return 0;
}