//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_' || c == '-';
}

int extract_words(const char *text, char words[][MAX_WORD_LEN], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i]) && (i - start) < MAX_WORD_LEN - 1) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            strncpy(words[word_count], text + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount *word_counts) {
    if (words == NULL || word_counts == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp[MAX_WORD_LEN];
        strncpy(temp, words[i], MAX_WORD_LEN - 1);
        temp[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp);
        
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(word_counts[j].word, temp) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, temp, MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (word_counts[j].count < word_counts[j + 1].count) {
                WordCount temp = word_counts[j];
                word_counts[j] = word_counts[j + 1];
                word_counts[j + 1] = temp;
            }
        }
    }
}

void process_text(const char *text) {
    if (!is_valid_input(text)) {
        printf("Invalid input text\n");
        return;
    }
    
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = extract_words(text, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No words found in text\n");
        return;
    }
    
    WordCount word_counts[MAX_WORDS];
    int unique_count = count_unique_words(words, word_count, word_counts);
    
    sort_word_counts(word_counts, unique_count);
    
    printf("Word frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    printf("Top words:\n");
    
    int display_count = (unique_count < 10) ? unique_count : 10;
    for (int i = 0; i < display_count; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_text(input);