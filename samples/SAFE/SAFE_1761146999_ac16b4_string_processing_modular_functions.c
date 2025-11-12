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

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_INPUT_LEN) return 0;
    return 1;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c) || c == '\'' || c == '-';
}

int extract_words(const char* text, char words[][MAX_WORD_LEN], int max_words) {
    if (!is_valid_input(text)) return 0;
    
    int word_count = 0;
    int i = 0;
    int len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) i++;
        
        if (i >= len) break;
        
        int start = i;
        while (i < len && is_word_char(text[i])) i++;
        
        int word_len = i - start;
        if (word_len >= MAX_WORD_LEN) word_len = MAX_WORD_LEN - 1;
        
        strncpy(words[word_count], text + start, word_len);
        words[word_count][word_len] = '\0';
        
        for (int j = 0; j < word_len; j++) {
            words[word_count][j] = tolower((unsigned char)words[word_count][j]);
        }
        
        word_count++;
    }
    
    return word_count;
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
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int index = find_word_index(unique_counts, unique_count, words[i]);
        if (index >= 0) {
            if (unique_counts[index].count < INT_MAX) {
                unique_counts[index].count++;
            }
        } else if (unique_count < MAX_WORDS) {
            strncpy(unique_counts[unique_count].word, words[i], MAX_WORD_LEN - 1);
            unique_counts[unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount* counts, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (counts[j].count < counts[j + 1].count) {
                struct WordCount temp = counts[j];
                counts[j] = counts[j + 1];
                counts[j + 1] = temp;
            }
        }
    }
}

void print_word_counts(struct WordCount* counts, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-15s: %d\n", counts[i].word, counts[i].count);
    }
}

int main(void) {
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
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    int unique_count = count_unique_words(words, word_count, unique_counts);
    
    if (unique_count == 0) {
        printf("No unique words found.\n");
        return 0;
    }
    
    sort_word_counts(unique_counts, unique_count);
    print_word_counts(unique_counts, unique_count);
    
    return 0;
}