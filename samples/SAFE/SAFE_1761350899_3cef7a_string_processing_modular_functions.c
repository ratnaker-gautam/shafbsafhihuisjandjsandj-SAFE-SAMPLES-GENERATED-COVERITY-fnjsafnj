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
    if (!is_valid_input(text)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    to_lowercase(buffer);
    
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
        
        size_t word_len = i - start;
        if (word_len >= sizeof(words[*word_count].word)) {
            word_len = sizeof(words[*word_count].word) - 1;
        }
        
        strncpy(words[*word_count].word, &buffer[start], word_len);
        words[*word_count].word[word_len] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    return 1;
}

int find_word_index(const struct WordCount words[], int word_count, const char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(const char *text, struct WordCount words[], int *unique_count) {
    if (!extract_words(text, words, unique_count)) return 0;
    
    for (int i = 0; i < *unique_count; i++) {
        for (int j = i + 1; j < *unique_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < *unique_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                (*unique_count)--;
                j--;
            }
        }
    }
    return 1;
}

void print_word_counts(const struct WordCount words[], int word_count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int unique_count = 0;
    
    printf("Enter text to analyze: ");
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
    
    if (!count_unique_words(input, words, &unique_count)) {
        fprintf(stderr, "Error processing text\n");
        return 1;
    }
    
    if (unique_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    print_word_counts(words, unique_count);
    return 0;
}