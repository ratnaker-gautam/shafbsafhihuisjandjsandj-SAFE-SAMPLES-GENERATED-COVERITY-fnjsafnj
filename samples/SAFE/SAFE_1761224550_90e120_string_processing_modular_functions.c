//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
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
            
            for (size_t j = 0; j < word_len; j++) {
                words[word_count][j] = tolower(words[word_count][j]);
            }
            
            word_count++;
        }
    }
    
    return word_count;
}

int find_word_index(struct WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *word_counts) {
    if (word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(word_counts, unique_count, words[i]);
        if (index == -1) {
            if (unique_count < MAX_WORDS) {
                strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
                word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                word_counts[unique_count].count = 1;
                unique_count++;
            }
        } else {
            if (word_counts[index].count < INT_MAX) {
                word_counts[index].count++;
            }
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *word_counts, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (word_counts[j].count < word_counts[j + 1].count) {
                struct WordCount temp = word_counts[j];
                word_counts[j] = word_counts[j + 1];
                word_counts[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(struct WordCount *word_counts, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount word_counts[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input detected.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts);
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 1;
    }
    
    sort_word_counts(word_counts, unique_count);
    print_word_counts(word_counts, unique_count);
    
    return 0;
}