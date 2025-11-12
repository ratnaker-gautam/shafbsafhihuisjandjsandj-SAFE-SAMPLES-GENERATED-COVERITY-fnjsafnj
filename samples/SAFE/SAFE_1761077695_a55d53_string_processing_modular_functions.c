//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char word[64];
    int count;
};

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int extract_words(const char* text, char words[][64], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !isalpha(text[i])) i++;
        
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && isalpha(text[i])) i++;
        
        size_t word_len = i - start;
        if (word_len >= 63) word_len = 62;
        
        for (size_t j = 0; j < word_len; j++) {
            words[word_count][j] = tolower(text[start + j]);
        }
        words[word_count][word_len] = '\0';
        
        if (word_len > 0) {
            word_count++;
        }
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

void count_words(char words[][64], int word_count, struct WordCount* counts, int* unique_count) {
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int idx = find_word_index(counts, *unique_count, words[i]);
        if (idx >= 0) {
            if (counts[idx].count < 1000000) {
                counts[idx].count++;
            }
        } else if (*unique_count < MAX_WORDS) {
            strncpy(counts[*unique_count].word, words[i], 63);
            counts[*unique_count].word[63] = '\0';
            counts[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
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

void print_results(struct WordCount* counts, int count) {
    printf("Word frequency analysis:\n");
    printf("------------------------\n");
    
    for (int i = 0; i < count && i < 10; i++) {
        printf("%-15s: %d\n", counts[i].word, counts[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
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
        printf("No words found in input\n");
        return 0;
    }
    
    int unique_count;
    count_words(words, word_count, word_counts, &unique_count);
    
    if (unique_count > 0) {
        sort_word_counts(word_counts, unique_count);
        print_results(word_counts, unique_count);
    }
    
    return 0;
}