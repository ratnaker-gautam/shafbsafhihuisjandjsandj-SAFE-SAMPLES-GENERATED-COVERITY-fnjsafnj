//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int extract_words(const char *text, struct WordCount words[], int max_words) {
    if (text == NULL || words == NULL || max_words <= 0) {
        return 0;
    }
    
    int word_count = 0;
    int text_len = (int)strlen(text);
    int word_start = -1;
    
    for (int i = 0; i <= text_len; i++) {
        char current = (i < text_len) ? text[i] : ' ';
        
        if (is_valid_char(current)) {
            if (word_start == -1) {
                word_start = i;
            }
        } else {
            if (word_start != -1) {
                int word_len = i - word_start;
                if (word_len > 0 && word_len < MAX_WORD_LEN && word_count < max_words) {
                    strncpy(words[word_count].word, &text[word_start], word_len);
                    words[word_count].word[word_len] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
                word_start = -1;
            }
        }
    }
    
    return word_count;
}

int find_word_index(const struct WordCount words[], int count, const char *word) {
    if (words == NULL || word == NULL) {
        return -1;
    }
    
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void count_word_frequencies(struct WordCount words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) {
        return;
    }
    
    struct WordCount unique_words[MAX_WORDS];
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        int found_idx = find_word_index(unique_words, unique_count, words[i].word);
        if (found_idx >= 0) {
            unique_words[found_idx].count++;
        } else if (unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i].word, MAX_WORD_LEN - 1);
            unique_words[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    for (int i = 0; i < unique_count; i++) {
        strncpy(words[i].word, unique_words[i].word, MAX_WORD_LEN - 1);
        words[i].word[MAX_WORD_LEN - 1] = '\0';
        words[i].count = unique_words[i].count;
    }
    
    *word_count = unique_count;
}

void to_lowercase_words(struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        return;
    }
    
    for (int i = 0; i < count; i++) {
        for (int j = 0; words[i].word[j] != '\0'; j++) {
            words[i].word[j] = tolower((unsigned char)words[i].word[j]);
        }
    }
}

void sort_words_by_frequency(struct WordCount words[], int count) {
    if (words == NULL || count <= 1) {
        return;
    }
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count || 
                (words[j].count == words[j + 1].count && strcmp(words[j].word, words[j + 1].word) > 0)) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

void print_word_frequencies(const struct WordCount words[], int count) {
    if (words == NULL || count <= 0) {
        printf("No words to display.\n");
        return;
    }
    
    printf("Word frequencies (sorted by count):\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-20s: %d\n", words[i].word, words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\