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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (input == NULL || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        char c = input[i];
        if (is_word_char(c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    to_lowercase(words[word_count]);
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int find_word_index(struct WordCount *word_counts, int count, const char *word) {
    if (word_counts == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *unique_words) {
    if (words == NULL || unique_words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int idx = find_word_index(unique_words, unique_count, words[i]);
        if (idx >= 0) {
            unique_words[idx].count++;
        } else {
            if (unique_count < MAX_WORDS) {
                strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LEN - 1);
                unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
                unique_words[unique_count].count = 1;
                unique_count++;
            }
        }
    }
    
    return unique_count;
}

void print_word_counts(struct WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_words[MAX_WORDS];
    
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
    if (word_count <= 0) {
        printf("No valid words found in input\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    if (unique_count <= 0) {
        printf("Error processing words\n");
        return 1;
    }
    
    print_word_counts(unique_words, unique_count);
    
    return 0;
}