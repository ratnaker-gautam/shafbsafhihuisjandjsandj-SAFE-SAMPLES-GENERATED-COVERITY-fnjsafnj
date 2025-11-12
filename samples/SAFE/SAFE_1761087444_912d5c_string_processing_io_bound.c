//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int extract_words(const char *input, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) {
        return 0;
    }
    
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    int count = 0;
    char *token = buffer;
    
    while (*token != '\0' && count < MAX_WORDS) {
        while (*token != '\0' && !is_valid_char(*token)) {
            token++;
        }
        
        if (*token == '\0') {
            break;
        }
        
        char *start = token;
        while (*token != '\0' && is_valid_char(*token)) {
            token++;
        }
        
        size_t len = token - start;
        if (len >= MAX_WORD_LEN) {
            len = MAX_WORD_LEN - 1;
        }
        
        char word[MAX_WORD_LEN];
        strncpy(word, start, len);
        word[len] = '\0';
        to_lower_case(word);
        
        if (strlen(word) > 0) {
            int index = find_word_index(words, count, word);
            if (index >= 0) {
                words[index].count++;
            } else {
                if (count < MAX_WORDS) {
                    strncpy(words[count].word, word, MAX_WORD_LEN - 1);
                    words[count].word[MAX_WORD_LEN - 1] = '\0';
                    words[count].count = 1;
                    count++;
                }
            }
        }
        
        if (*token != '\0') {
            token++;
        }
    }
    
    *word_count = count;
    return 1;
}

void sort_words(struct WordCount *words, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(words[j].word, words[j + 1].word) > 0) {
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
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}