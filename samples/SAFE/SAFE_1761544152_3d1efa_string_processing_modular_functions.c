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
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < max_words) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    for (size_t j = 0; j < word_len; j++) {
                        words[word_count][j] = tolower(words[word_count][j]);
                    }
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    return word_count;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *word_counts) {
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    return unique_count;
}

void sort_word_counts(struct WordCount *word_counts, int count) {
    if (count <= 1) return;
    
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

void print_word_counts(const struct WordCount *word_counts, int count) {
    if (count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount word_counts[MAX_WORDS];
    
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
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count, word_counts);
    if (unique_count == 0) {
        printf("Error processing words.\n");
        return 1;
    }
    
    sort_word_counts(word_counts, unique_count);
    print_word_counts(word_counts, unique_count);
    
    return 0;
}