//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    if (strlen(input) >= MAX_INPUT_LENGTH) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char* text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char* ptr = text;
    
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        const char* start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        
        size_t len = ptr - start;
        if (len >= sizeof(words[0].word)) len = sizeof(words[0].word) - 1;
        
        strncpy(words[word_count].word, start, len);
        words[word_count].word[len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (char* p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
}

int find_word_index(struct WordCount words[], int count, const char* word) {
    if (words == NULL || word == NULL) return -1;
    
    char temp[64];
    strncpy(temp, word, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    to_lowercase(temp);
    
    for (int i = 0; i < count; i++) {
        char compare[64];
        strncpy(compare, words[i].word, sizeof(compare) - 1);
        compare[sizeof(compare) - 1] = '\0';
        to_lowercase(compare);
        
        if (strcmp(temp, compare) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
        int found_index = find_word_index(words, i, words[i].word);
        
        if (found_index == -1) {
            if (unique_count != i) {
                words[unique_count] = words[i];
            }
            unique_count++;
        } else {
            words[found_index].count++;
        }
    }
    
    return unique_count;
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

void display_word_counts(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LENGTH];
    struct WordCount words[MAX_WORDS];
    
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
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count);
    
    if (unique_count > 0) {
        sort_words_by_count(words, unique_count);
        display_word_counts(words, unique_count);
    }
    
    return 0;
}