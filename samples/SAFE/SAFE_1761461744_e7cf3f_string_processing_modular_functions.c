//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char *input, char words[][64], int max_words) {
    if (!is_valid_input(input) || words == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len && word_count < max_words; i++) {
        unsigned char c = (i < input_len) ? (unsigned char)input[i] : ' ';
        
        if (isalpha(c)) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 64) {
                    strncpy(words[word_count], input + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    
                    for (size_t j = 0; j < word_len; j++) {
                        words[word_count][j] = tolower((unsigned char)words[word_count][j]);
                    }
                    
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    
    return word_count;
}

int count_unique_words(char words[][64], int word_count, struct WordCount *unique_words) {
    if (words == NULL || unique_words == NULL || word_count <= 0) return 0;
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique_words[j].word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            strncpy(unique_words[unique_count].word, words[i], 63);
            unique_words[unique_count].word[63] = '\0';
            unique_words[unique_count].count = 1;
            unique_count++;
        }
    }
    
    return unique_count;
}

void sort_word_counts(struct WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (words[j].count < words[j + 1].count) {
                struct WordCount temp = words[j];
                words[j] = words[j + 1];
                words[j + 1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    struct WordCount unique_words[MAX_WORDS];
    
    printf("Enter text to analyze: ");
    
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
    
    int unique_count = count_unique_words(words, word_count, unique_words);
    if (unique_count == 0) {
        printf("No unique words found\n");
        return 0;
    }
    
    sort_word_counts(unique_words, unique_count);
    
    printf("\nWord frequency analysis:\n");
    printf("Word\t\tCount\n");
    printf("----\t\t-----\n");
    
    for (int i = 0; i < unique_count && i < 10; i++) {
        printf("%-15s\t%d\n", unique_words[i].word, unique_words[i].count);
    }
    
    return 0;
}