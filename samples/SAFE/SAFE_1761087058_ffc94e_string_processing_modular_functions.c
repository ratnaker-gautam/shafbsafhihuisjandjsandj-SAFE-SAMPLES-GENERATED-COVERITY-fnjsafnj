//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LENGTH 1024
#define MAX_WORDS 100
#define MAX_WORD_LENGTH 50

struct WordCount {
    char word[MAX_WORD_LENGTH];
    int count;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    if (strlen(input) == 0) return 0;
    if (strlen(input) >= MAX_INPUT_LENGTH) return 0;
    return 1;
}

int is_word_character(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int extract_words(const char* input, char words[][MAX_WORD_LENGTH], int max_words) {
    if (!is_valid_input(input)) return 0;
    
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    int word_len = 0;
    
    while (input[pos] != '\0' && word_count < max_words) {
        if (is_word_character(input[pos])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LENGTH - 1) {
                words[word_count][word_len] = tolower(input[pos]);
                word_len++;
            }
        } else {
            if (in_word) {
                words[word_count][word_len] = '\0';
                if (word_len > 0) {
                    word_count++;
                }
                in_word = 0;
            }
        }
        pos++;
    }
    
    if (in_word && word_len > 0 && word_count < max_words) {
        words[word_count][word_len] = '\0';
        word_count++;
    }
    
    return word_count;
}

int find_word_index(struct WordCount* word_counts, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LENGTH], int word_count, struct WordCount* unique_words) {
    if (word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(unique_words, unique_count, words[i]);
        if (index == -1) {
            if (unique_count < MAX_WORDS) {
                strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LENGTH - 1);
                unique_words[unique_count].word[MAX_WORD_LENGTH - 1] = '\0';
                unique_words[unique_count].count = 1;
                unique_count++;
            }
        } else {
            unique_words[index].count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount* word_counts, int count) {
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

void print_word_counts(struct WordCount* word_counts, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LENGTH];
    char words[MAX_WORDS][MAX_WORD_LENGTH];
    struct WordCount unique_words[MAX_WORDS];
    
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
        printf("Invalid input.\n");
        return 1;
    }
    
    int word_count = extract_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input.\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    if (unique_count == 0) {
        printf("Error counting unique words.\n");
        return 1;
    }
    
    sort_word_counts(unique_words, unique_count);
    print_word_counts(unique_words, unique_count);
    
    return 0;
}