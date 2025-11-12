//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) == 0) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text) || words == NULL || word_count == NULL) return 0;
    
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_lowercase(buffer);
    
    *word_count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && *word_count < MAX_WORDS) {
        while (buffer[i] != '\0' && !is_word_char(buffer[i])) {
            i++;
        }
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (buffer[i] != '\0' && is_word_char(buffer[i])) {
            i++;
        }
        
        size_t length = i - start;
        if (length >= sizeof(words[*word_count].word)) {
            length = sizeof(words[*word_count].word) - 1;
        }
        
        strncpy(words[*word_count].word, &buffer[start], length);
        words[*word_count].word[length] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

int count_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count == 0) continue;
            
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                words[j].count = 0;
            }
        }
    }
    
    int new_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            words[new_count] = words[i];
            new_count++;
        }
    }
    
    return new_count;
}

void print_word_frequency(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        fprintf(stderr, "Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    word_count = count_word_frequency(words, word_count);
    
    print_word_frequency(words, word_count);
    
    return 0;
}