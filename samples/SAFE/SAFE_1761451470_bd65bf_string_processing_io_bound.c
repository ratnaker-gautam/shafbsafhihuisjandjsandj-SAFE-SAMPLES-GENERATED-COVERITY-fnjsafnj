//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int find_word(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, char words[][MAX_WORD_LEN], int max_words) {
    int word_count = 0;
    int pos = 0;
    int in_word = 0;
    char current_word[MAX_WORD_LEN] = {0};
    int current_len = 0;

    for (int i = 0; input[i] && word_count < max_words; i++) {
        if (is_valid_char(input[i])) {
            if (!in_word) {
                in_word = 1;
                current_len = 0;
            }
            if (current_len < MAX_WORD_LEN - 1) {
                current_word[current_len++] = input[i];
                current_word[current_len] = '\0';
            }
        } else {
            if (in_word && current_len > 0) {
                to_lower_case(current_word);
                strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
                words[word_count][MAX_WORD_LEN - 1] = '\0';
                word_count++;
                in_word = 0;
                current_len = 0;
            }
        }
    }

    if (in_word && current_len > 0 && word_count < max_words) {
        to_lower_case(current_word);
        strncpy(words[word_count], current_word, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
    }

    return word_count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount word_counts[MAX_WORDS];
    int unique_count = 0;

    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }

    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }

    if (len >= MAX_INPUT_LEN) {
        printf("Input too long, truncating...\n");
        input[MAX_INPUT_LEN - 1] = '\0';
    }

    int word_count = extract_words(input, words, MAX_WORDS);

    if (word_count == 0) {
        printf("No valid words found in input\n");
        return 1;
    }

    for (int i = 0; i < word_count; i++) {
        int found_index = find_word(word_counts, unique_count, words[i]);
        if (found_index >= 0) {
            word_counts[found_index].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(word_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            word_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }

    printf("\nWord frequency analysis:\n");
    printf("========================\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%-20s: %d\n", word_counts[i].word, word_counts[i].count);
    }

    printf("\nTotal words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);

    return 0;
}