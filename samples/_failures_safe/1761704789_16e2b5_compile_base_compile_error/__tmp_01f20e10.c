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
            strncpy(words[word_count].word, &text[start], word_len);
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
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    char temp[MAX_WORD_LEN];
    strncpy(temp, word, MAX_WORD_LEN - 1);
    temp[MAX_WORD_LEN - 1] = '\0';
    to_lowercase(temp);
    
    for (int i = 0; i < count; i++) {
        char current[MAX_WORD_LEN];
        strncpy(current, words[i].word, MAX_WORD_LEN - 1);
        current[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(current);
        
        if (strcmp(current, temp) == 0) {
            return i;
        }
    }
    
    return -1;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    struct WordCount unique_words[MAX_WORDS];
    int unique_count = 0;
    
    for (int i = 0; i < word_count && unique_count < MAX_WORDS; i++) {
        to_lowercase(words[i].word);
        int found_index = find_word_index(unique_words, unique_count, words[i].word);
        
        if (found_index == -1) {
            strncpy(unique_words[unique_count].word, words[i].word, MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        } else {
            unique_words[found_index].count++;
        }
    }
    
    for (int i = 0; i < unique_count; i++) {
        words[i] = unique_words[i];
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

void print_word_counts(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (words[i].word[0] != '\0') {
            printf("%-15s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main() {
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