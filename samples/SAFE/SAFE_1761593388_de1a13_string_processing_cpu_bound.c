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

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

void to_lower_case(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void sort_words(struct WordCount words[], int count) {
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
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr != '\0') {
        while (*ptr != '\0' && !is_word_char(*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr != '\0' && is_word_char(*ptr)) {
            ptr++;
        }
        
        char temp = *ptr;
        *ptr = '\0';
        
        if (word_count >= MAX_WORDS) {
            printf("Too many words, processing first %d words\n", MAX_WORDS);
            break;
        }
        
        to_lower_case(word_start);
        
        int index = find_word_index(words, word_count, word_start);
        if (index >= 0) {
            if (words[index].count < INT_MAX) {
                words[index].count++;
            }
        } else {
            if (strlen(word_start) < sizeof(words[0].word)) {
                strncpy(words[word_count].word, word_start, sizeof(words[0].word) - 1);
                words[word_count].word[sizeof(words[0].word) - 1] = '\0';
                words[word_count].count = 1;
                word_count++;
            }
        }
        
        *ptr = temp;
        if (*ptr != '\0') {
            ptr++;
        }
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}