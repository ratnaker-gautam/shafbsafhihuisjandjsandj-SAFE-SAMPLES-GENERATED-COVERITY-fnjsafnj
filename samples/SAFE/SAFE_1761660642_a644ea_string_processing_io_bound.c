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

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, struct WordCount words[], int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        return -1;
    }
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lowercase(buffer);
    
    int i = 0;
    while (buffer[i] != '\0') {
        while (buffer[i] != '\0' && !is_valid_char(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') {
            break;
        }
        
        int start = i;
        while (buffer[i] != '\0' && is_valid_char(buffer[i])) {
            i++;
        }
        
        int length = i - start;
        if (length >= MAX_WORD_LEN) {
            length = MAX_WORD_LEN - 1;
        }
        
        char word[MAX_WORD_LEN];
        strncpy(word, buffer + start, length);
        word[length] = '\0';
        
        if (strlen(word) > 0) {
            int index = find_word_index(words, *word_count, word);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else if (*word_count < MAX_WORDS) {
                strncpy(words[*word_count].word, word, MAX_WORD_LEN - 1);
                words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
    
    return 0;
}

void print_word_counts(struct WordCount words[], int count) {
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
        if (fgets(input, MAX_INPUT_LEN, stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            break;
        }
        
        if (process_input(input, words, &word_count) != 0) {
            fprintf(stderr, "Error processing input\n");
            return 1;
        }
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found.\n");
    }
    
    return 0;
}