//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int split_into_words(const char *text, char words[][MAX_WORD_LEN], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    int word_len = 0;
    
    while (text[pos] != '\0' && word_count < max_words) {
        if (is_valid_char(text[pos])) {
            if (!in_word) {
                in_word = 1;
                word_len = 0;
            }
            if (word_len < MAX_WORD_LEN - 1) {
                words[word_count][word_len] = tolower((unsigned char)text[pos]);
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

int find_word_index(struct WordCount *word_counts, int count, const char *word) {
    if (word_counts == NULL || word == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount *unique_words) {
    if (words == NULL || unique_words == NULL || word_count <= 0) {
        return 0;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(unique_words, unique_count, words[i]);
        if (index >= 0) {
            unique_words[index].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *word_counts, int count) {
    if (word_counts == NULL || count <= 1) {
        return;
    }
    
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
    if (word_counts == NULL || count <= 0) {
        return;
    }
    
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count; i++) {
        if (word_counts[i].word[0] != '\0') {
            printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
        }
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = split_into_words(input, words, MAX_WORDS);
    
    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    
    if (unique_count == 0) {
        printf("No unique words found\n");
        return 1;
    }
    
    sort_word_counts(unique_words, unique_count);
    print_word_counts(unique_words, unique_count);
    
    return 0;
}