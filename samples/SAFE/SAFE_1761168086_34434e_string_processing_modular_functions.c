//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 100
#define MAX_WORDS 1000

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int i = 0;
    int len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_valid_char(text[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_valid_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        strncpy(words[word_count].word, text + start, word_len);
        words[word_count].word[word_len] = '\0';
        words[word_count].count = 1;
        word_count++;
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
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
    if (words == NULL || word_count <= 1) {
        return;
    }
    
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
    if (words == NULL || word_count <= 0) {
        printf("No words to display.\n");
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("========================\n");
    
    int displayed = 0;
    for (int i = 0; i < word_count && displayed < 10; i++) {
        if (words[i].count > 0) {
            printf("%-15s: %d\n", words[i].word, words[i].count);
            displayed++;
        }
    }
}

int main(void) {
    char input_text[5000];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis (max %d characters):\n", 4999);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input_text);
    if (len > 0 && input_text[len - 1] == '\n') {
        input_text[len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int total_words = extract_words(input_text, words, MAX_WORDS);
    
    if (total_words == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    int unique_words = count_unique_words(words, total_words);
    
    printf("\nTotal words: %d\n", total_words);
    printf("Unique words: %d\n\n", unique_words);
    
    sort_words_by_count(words, total_words);
    print_word_stats(words, total_words);
    
    return 0;
}