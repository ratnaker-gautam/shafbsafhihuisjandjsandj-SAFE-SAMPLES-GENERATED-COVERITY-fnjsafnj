//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
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
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    char temp[MAX_INPUT_LEN];
    
    if (strlen(input) >= sizeof(temp)) return 0;
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    to_lowercase(temp);
    
    while (temp[i] != '\0' && word_count < max_words) {
        while (temp[i] != '\0' && !is_word_char(temp[i])) i++;
        if (temp[i] == '\0') break;
        
        size_t start = i;
        while (temp[i] != '\0' && is_word_char(temp[i])) i++;
        size_t length = i - start;
        
        if (length > 0 && length < MAX_WORD_LEN) {
            strncpy(words[word_count], &temp[start], length);
            words[word_count][length] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *unique_words) {
    if (words == NULL || word_count <= 0 || unique_words == NULL) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void print_word_counts(struct WordCount *unique_words, int unique_count) {
    if (unique_words == NULL || unique_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i].word, unique_words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count <= 0) {
        printf("No valid words found.\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    if (unique_count <= 0) {
        printf("Error counting words.\n");
        return 1;
    }
    
    print_word_counts(unique_words, unique_count);
    
    return 0;
}