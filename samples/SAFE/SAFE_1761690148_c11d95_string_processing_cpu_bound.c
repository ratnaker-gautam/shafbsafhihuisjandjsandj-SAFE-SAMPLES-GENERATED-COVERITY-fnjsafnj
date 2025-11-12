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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    return 0;
}

void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int main(void) {
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
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char current_word[50];
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < (int)sizeof(current_word) - 1) {
            current_word[word_pos++] = c;
            in_word = 1;
        } else {
            if (in_word) {
                current_word[word_pos] = '\0';
                to_lowercase(current_word);
                process_word(current_word, words, &word_count);
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    int total_words = 0;
    for (int i = 0; i < word_count; i++) {
        total_words += words[i].count;
    }
    printf("\nTotal words: %d\n", total_words);
    printf("Unique words: %d\n", word_count);
    
    return 0;
}