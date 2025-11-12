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
            strncpy(words[word_count].word, text + start, (size_t)word_len);
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

int find_word_index(struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return 0;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i].word);
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i].count == 0) continue;
        
        int found_index = find_word_index(words, i, words[i].word);
        if (found_index == i) {
            unique_count++;
        } else if (found_index >= 0) {
            words[found_index].count++;
            words[i].count = 0;
        }
    }
    
    return unique_count;
}

void print_word_counts(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].count > 0) {
            printf("%s: %d\n", words[i].word, words[i].count);
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
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int extracted_count = extract_words(input, words, MAX_WORDS);
    if (extracted_count <= 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, extracted_count);
    
    if (unique_count > 0) {
        print_word_counts(words, extracted_count);
        printf("Total unique words: %d\n", unique_count);
    } else {
        printf("No unique words found\n");
    }
    
    return 0;
}