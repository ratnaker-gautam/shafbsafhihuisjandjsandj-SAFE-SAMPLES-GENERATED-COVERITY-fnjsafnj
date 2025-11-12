//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

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
    char temp_word[MAX_WORD_LEN];
    int buf_len = strlen(input);
    
    if (buf_len >= MAX_INPUT_LEN - 1) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    to_lower_case(buffer);
    
    int pos = 0;
    int temp_pos = 0;
    
    while (pos < buf_len && *word_count < MAX_WORDS) {
        while (pos < buf_len && !is_valid_char(buffer[pos])) {
            pos++;
        }
        
        if (pos >= buf_len) {
            break;
        }
        
        temp_pos = 0;
        while (pos < buf_len && is_valid_char(buffer[pos]) && temp_pos < MAX_WORD_LEN - 1) {
            temp_word[temp_pos++] = buffer[pos++];
        }
        temp_word[temp_pos] = '\0';
        
        if (temp_pos > 0) {
            int index = find_word_index(words, *word_count, temp_word);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (*word_count < MAX_WORDS) {
                    strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN - 1);
                    words[*word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
    }
    
    return 1;
}

void print_word_counts(struct WordCount words[], int count) {
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main() {
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
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
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    printf("\nWord frequencies:\n");
    print_word_counts(words, word_count);
    
    return 0;
}