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
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text)) return 0;
    
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    *word_count = 0;
    int pos = 0;
    int len = strlen(buffer);
    
    while (pos < len && *word_count < MAX_WORDS) {
        while (pos < len && !is_word_char(buffer[pos])) pos++;
        if (pos >= len) break;
        
        int start = pos;
        while (pos < len && is_word_char(buffer[pos])) pos++;
        
        int word_len = pos - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(words[*word_count].word, &buffer[start], word_len);
        words[*word_count].word[word_len] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int count_unique_words(struct WordCount words[], int word_count) {
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        unique_count++;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > 0 && strcmp(words[i].word, words[j].word) == 0) {
                words[i].count += words[j].count;
                words[j].count = 0;
            }
        }
    }
    
    return unique_count;
}

void sort_words_by_count(struct WordCount words[], int word_count) {
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_stats(struct WordCount words[], int word_count) {
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    int printed = 0;
    for (int i = 0; i < word_count && printed < 10; i++) {
        if (words[i].count > 0) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
            printed++;
        }
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!extract_words(input, words, &word_count)) {
        printf("Invalid input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int unique_words = count_unique_words(words, word_count);
    sort_words_by_count(words, word_count);
    
    printf("\nTotal words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_words);
    print_word_stats(words, word_count);
    
    return 0;
}