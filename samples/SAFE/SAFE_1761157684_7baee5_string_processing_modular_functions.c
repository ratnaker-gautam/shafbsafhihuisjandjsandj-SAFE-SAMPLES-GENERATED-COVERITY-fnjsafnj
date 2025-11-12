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

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int len = strlen(text);
    int i = 0;
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) {
            i++;
        }
        
        if (i >= len) {
            break;
        }
        
        int start = i;
        while (i < len && is_word_char(text[i])) {
            i++;
        }
        
        int word_len = i - start;
        if (word_len >= 64) {
            word_len = 63;
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
        str[i] = tolower(str[i]);
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
        if (words[i].count == 0) {
            continue;
        }
        
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 1;
    }
    
    int extracted_count = extract_words(input, words, MAX_WORDS);
    if (extracted_count == 0) {
        printf("No words found in input.\n");
        return 1;
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
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", extracted_count);
    printf("Unique words: %d\n\n", unique_count);
    
    int display_count = unique_count > 10 ? 10 : unique_count;
    for (int i = 0; i < display_count; i++) {
        printf("%-20s: %d\n", unique_words[i].word, unique_words[i].count);
    }
    
    if (unique_count > 10) {
        printf("... and %d more words\n", unique_count - 10);
    }
    
    return 0;
}