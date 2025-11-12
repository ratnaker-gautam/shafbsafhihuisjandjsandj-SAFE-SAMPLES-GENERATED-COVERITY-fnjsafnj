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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int text_len = (int)strlen(text);
    int i = 0;
    
    while (i < text_len && word_count < max_words) {
        while (i < text_len && !is_valid_char(text[i])) {
            i++;
        }
        
        if (i >= text_len) {
            break;
        }
        
        int start = i;
        while (i < text_len && is_valid_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count].word, text + start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
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
    if (words == NULL || word_count <= 1) return;
    
    for (int i = 0; i < word_count - 1; i++) {
        if (words[i].count == 0) continue;
        
        for (int j = i + 1; j < word_count; j++) {
            if (words[j].count > 0 && 
                (words[j].count > words[i].count || 
                 (words[j].count == words[i].count && strcmp(words[j].word, words[i].word) < 0))) {
                struct WordCount temp = words[i];
                words[i] = words[j];
                words[j] = temp;
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
    
    if (displayed == 0) {
        printf("No valid words found.\n");
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in the input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 1;
    }
    
    sort_words_by_count(words, word_count);
    print_word_stats(words, word_count);
    
    return 0;
}