//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char* str) {
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

int extract_words(const char* text, char words[][MAX_WORD_LEN], int max_words) {
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

void normalize_word(char* word) {
    if (word == NULL) return;
    for (size_t i = 0; word[i] != '\0'; i++) {
        word[i] = tolower((unsigned char)word[i]);
    }
}

int find_word_index(WordCount* word_counts, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, WordCount* unique_words) {
    if (words == NULL || unique_words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char normalized[MAX_WORD_LEN];
        strncpy(normalized, words[i], MAX_WORD_LEN - 1);
        normalized[MAX_WORD_LEN - 1] = '\0';
        normalize_word(normalized);
        
        int index = find_word_index(unique_words, unique_count, normalized);
        if (index >= 0) {
            unique_words[index].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, normalized, MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(WordCount* word_counts, int count) {
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

void print_word_counts(const WordCount* word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count; i++) {
        if (i < 10) {
            printf("%2d. %-20s: %d\n", i + 1, word_counts[i].word, word_counts[i].count);
        } else {
            printf("    %-20s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount unique_words[MAX_WORDS];
    
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
    
    int word_count = extract_words(input,