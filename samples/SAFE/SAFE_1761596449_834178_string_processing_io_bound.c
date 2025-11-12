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
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int pos = 0;
    char current_word[MAX_WORD_LEN];
    int word_len = 0;
    
    while (buffer[pos] != '\0' && *word_count < MAX_WORDS) {
        if (is_valid_char(buffer[pos])) {
            if (word_len < MAX_WORD_LEN - 1) {
                current_word[word_len++] = buffer[pos];
            }
        } else {
            if (word_len > 0) {
                current_word[word_len] = '\0';
                to_lower_case(current_word);
                
                int existing_index = find_word_index(words, *word_count, current_word);
                if (existing_index != -1) {
                    words[existing_index].count++;
                } else {
                    if (*word_count < MAX_WORDS) {
                        strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
                        words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_len = 0;
            }
        }
        pos++;
    }
    
    if (word_len > 0 && *word_count < MAX_WORDS) {
        current_word[word_len] = '\0';
        to_lower_case(current_word);
        
        int existing_index = find_word_index(words, *word_count, current_word);
        if (existing_index != -1) {
            words[existing_index].count++;
        } else {
            strncpy(words[*word_count].word, current_word, MAX_WORD_LEN - 1);
            words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
    
    return 1;
}

void sort_words(struct WordCount words[], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s\t%d\n", words[i].word, words[i].count);
    }
    
    return 0;
}