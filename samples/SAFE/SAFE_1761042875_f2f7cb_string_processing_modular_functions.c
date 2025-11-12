//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(input);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(input[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(input[i]) && (i - start) < (MAX_WORD_LEN - 1)) i++;
        
        size_t word_len = i - start;
        if (word_len > 0) {
            strncpy(words[word_count], input + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void normalize_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        word[i] = tolower((unsigned char)word[i]);
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *word_counts) {
    if (word_count <= 0 || word_counts == NULL) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char normalized[MAX_WORD_LEN];
        strncpy(normalized, words[i], MAX_WORD_LEN - 1);
        normalized[MAX_WORD_LEN - 1] = '\0';
        normalize_word(normalized);
        
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(word_counts[j].word, normalized) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, normalized, MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void print_word_counts(const struct WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount word_counts[MAX_WORDS];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts);
    if (unique_count == 0) {
        printf("Error counting words\n");
        return 1;
    }
    
    print_word_counts(word_counts, unique_count);
    
    return 0;
}