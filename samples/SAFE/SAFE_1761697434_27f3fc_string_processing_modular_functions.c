//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE_LEN 256
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum(c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    char buffer[MAX_LINE_LEN];
    size_t text_len = strlen(text);
    if (text_len >= MAX_LINE_LEN) {
        return 0;
    }
    
    strncpy(buffer, text, MAX_LINE_LEN - 1);
    buffer[MAX_LINE_LEN - 1] = '\0';
    
    int word_count = 0;
    char *ptr = buffer;
    
    while (*ptr && word_count < max_words) {
        while (*ptr && !is_valid_char(*ptr)) {
            ptr++;
        }
        
        if (!*ptr) {
            break;
        }
        
        char *word_start = ptr;
        while (*ptr && is_valid_char(*ptr)) {
            ptr++;
        }
        
        size_t word_len = ptr - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count].word, word_start, word_len);
            words[word_count].word[word_len] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

int count_unique_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < word_count - 1; k++) {
                    strcpy(words[k].word, words[k + 1].word);
                    words[k].count = words[k + 1].count;
                }
                word_count--;
                j--;
            }
        }
    }
    
    return word_count;
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

void to_lowercase_words(struct WordCount words[], int word_count) {
    if (words == NULL || word_count <= 0) {
        return;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; words[i].word[j]; j++) {
            words[i].word[j] = tolower(words[i].word[j]);
        }
    }
}

int main(void) {
    char input_text[MAX_LINE_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters): ", MAX_LINE_LEN - 1);
    
    if (fgets(input_text, sizeof(input_text), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input_text);
    if (input_len > 0 && input_text[input_len - 1] == '\n') {
        input_text[input_len - 1] = '\0';
    }
    
    if (strlen(input_text) == 0) {
        printf("No input provided.\n");
        return 1;
    }
    
    int initial_count = extract_words(input_text, words, MAX_WORDS);
    if (initial_count <= 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    to_lowercase_words(words, initial_count);
    int unique_count = count_unique_words(words, initial_count);
    
    if (unique_count <= 0) {
        printf("Error processing words.\n");
        return 1;
    }
    
    sort_words_by_count(words, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("%-20s %s\n", "Word", "Count");
    printf("%-20s %s\n", "----", "-----");
    
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-20s %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}