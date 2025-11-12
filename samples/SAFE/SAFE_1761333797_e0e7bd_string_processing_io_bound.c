//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
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

int main() {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    
    while (token != NULL && word_count < MAX_WORDS) {
        int token_len = strlen(token);
        
        int start = 0;
        int end = token_len - 1;
        
        while (start < token_len && !is_word_char(token[start])) {
            start++;
        }
        while (end >= 0 && !is_word_char(token[end])) {
            end--;
        }
        
        if (start <= end) {
            int word_len = end - start + 1;
            if (word_len >= 63) {
                word_len = 62;
            }
            
            char clean_word[64];
            strncpy(clean_word, token + start, word_len);
            clean_word[word_len] = '\0';
            to_lower_case(clean_word);
            
            if (strlen(clean_word) > 0) {
                int index = find_word_index(words, word_count, clean_word);
                if (index >= 0) {
                    words[index].count++;
                } else {
                    strncpy(words[word_count].word, clean_word, sizeof(words[word_count].word) - 1);
                    words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
    }
    
    return 0;
}