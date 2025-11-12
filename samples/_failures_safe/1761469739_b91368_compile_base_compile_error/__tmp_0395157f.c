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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
        while (i < len && is_word_char(text[i]) && (i - start) < (size_t)(MAX_WORD_LEN - 1)) i++;
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len < (size_t)MAX_WORD_LEN) {
            strncpy(words[word_count], text + start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    return word_count;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int find_word_index(struct WordCount* counts, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* unique_counts) {
    if (words == NULL || unique_counts == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LEN];
        strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
        temp_word[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp_word);
        
        int index = find_word_index(unique_counts, unique_count, temp_word);
        if (index >= 0) {
            unique_counts[index].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(unique_counts[unique_count].word, temp_word, MAX_WORD_LEN - 1);
            unique_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount* counts, int count) {
    if (counts == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (counts[j].count < counts[j + 1].count || 
               (counts[j].count == counts[j + 1].count && strcmp(counts[j].word, counts[j + 1].word) > 0)) {
                struct WordCount temp = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(const struct WordCount* counts, int count) {
    if (counts == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", counts[i].word, counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_counts[MAX_WORDS];
    
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