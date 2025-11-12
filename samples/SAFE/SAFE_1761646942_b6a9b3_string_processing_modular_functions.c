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
    char *token = strtok(buffer, " \t\n\r.,;:!?\"");
    
    while (token != NULL && word_count < max_words) {
        size_t len = strlen(token);
        if (len > 0 && len < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                for (size_t i = 0; i < len; i++) {
                    token[i] = tolower((unsigned char)token[i]);
                }
                
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i].word, token) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    strncpy(words[word_count].word, token, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"");
    }
    
    return word_count;
}

void sort_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
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

void print_word_frequencies(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequencies (sorted by count, then alphabetically):\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    sort_words(words, word_count);
    print_word_frequencies(words, word_count);
    
    return 0;
}