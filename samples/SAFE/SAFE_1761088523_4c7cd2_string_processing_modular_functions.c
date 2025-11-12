//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char* text, struct WordCount words[], int max_words) {
    if (!is_valid_input(text) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    const char* p = text;
    
    while (*p && word_count < max_words) {
        while (*p && !is_word_char(*p)) p++;
        if (!*p) break;
        
        const char* start = p;
        while (*p && is_word_char(*p)) p++;
        
        size_t word_len = p - start;
        if (word_len >= sizeof(words[0].word)) word_len = sizeof(words[0].word) - 1;
        
        memcpy(words[word_count].word, start, word_len);
        words[word_count].word[word_len] = '\0';
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

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
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
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > words[i].count) {
                struct WordCount temp = words[i];
                words[i] = words[j];
                words[j] = temp;
            }
        }
    }
}

void print_word_stats(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%-20s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
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
    
    int extracted_count = extract_words(input, words, MAX_WORDS);
    if (extracted_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, extracted_count);
    
    struct WordCount unique_words[MAX_WORDS];
    int unique_index = 0;
    
    for (int i = 0; i < extracted_count && unique_index < MAX_WORDS; i++) {
        if (words[i].count > 0) {
            unique_words[unique_index] = words[i];
            unique_index++;
        }
    }
    
    sort_words_by_count(unique_words, unique_count);
    print_word_stats(unique_words, unique_count);
    
    return 0;
}