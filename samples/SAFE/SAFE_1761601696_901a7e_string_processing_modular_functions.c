//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) == 0) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (!is_valid_input(text) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char *p = text;
    
    while (*p && word_count < max_words) {
        while (*p && !is_word_char(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        while (*p && is_word_char(*p)) p++;
        
        size_t len = p - start;
        if (len >= sizeof(words[word_count].word)) len = sizeof(words[word_count].word) - 1;
        
        strncpy(words[word_count].word, start, len);
        words[word_count].word[len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (char *p = str; *p; p++) {
        *p = tolower(*p);
    }
}

int merge_duplicate_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return count;
    
    for (int i = 0; i < count; i++) {
        to_lowercase(words[i].word);
        for (int j = i + 1; j < count; j++) {
            to_lowercase(words[j].word);
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count += words[j].count;
                for (int k = j; k < count - 1; k++) {
                    words[k] = words[k + 1];
                }
                count--;
                j--;
            }
        }
    }
    
    return count;
}

void sort_words_by_count(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_stats(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 1;
    }
    
    word_count = merge_duplicate_words(words, word_count);
    sort_words_by_count(words, word_count);
    print_word_stats(words, word_count);
    
    return 0;
}