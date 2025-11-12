//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int *word_count) {
    if (!is_valid_input(text)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, text, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *ptr = buffer;
    while (*ptr && *word_count < MAX_WORDS) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len >= sizeof(words[*word_count].word)) len = sizeof(words[*word_count].word) - 1;
        
        strncpy(words[*word_count].word, start, len);
        words[*word_count].word[len] = '\0';
        words[*word_count].count = 1;
        (*word_count)++;
    }
    
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
}

int count_word_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > 0 && strcmp(words[i].word, words[j].word) == 0) {
                words[i].count += words[j].count;
                words[j].count = 0;
            }
        }
        unique_count++;
    }
    
    return unique_count;
}

void sort_words_by_frequency(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > words[i].count || 
               (words[j].count == words[i].count && strcmp(words[j].word, words[i].word) < 0)) {
                struct WordCount temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

void print_word_frequencies(const struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    printf("-----------------\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
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
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing words.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int unique_count = count_word_frequency(words, word_count);
    if (unique_count == 0) {
        printf("Error counting word frequencies.\n");
        return 1;
    }
    
    sort_words_by_frequency(words, word_count);
    print_word_frequencies(words, unique_count);
    
    return 0;
}