//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

struct WordInfo {
    char word[MAX_WORD_LEN + 1];
    int count;
    int first_position;
};

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = safe_strlen(input, MAX_INPUT_LEN + 1);
    if (len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int extract_words(const char *input, struct WordInfo words[], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    
    while (i < input_len && *word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && word_len < MAX_WORD_LEN && 
               !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            word_len++;
            i++;
        }
        
        if (word_len > 0) {
            size_t copy_len = word_len;
            if (copy_len > MAX_WORD_LEN) copy_len = MAX_WORD_LEN;
            
            for (size_t j = 0; j < copy_len; j++) {
                words[*word_count].word[j] = input[word_start + j];
            }
            words[*word_count].word[copy_len] = '\0';
            to_lowercase(words[*word_count].word);
            words[*word_count].count = 1;
            words[*word_count].first_position = *word_count;
            (*word_count)++;
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    return 1;
}

int find_word_index(struct WordInfo words[], int word_count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_word_frequency(struct WordInfo words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count <= 0) return;
    
    for (int i = 0; i < *word_count; i++) {
        for (int j = i + 1; j < *word_count; j++) {
            if (strcmp(words[i].word, words[j].word) == 0) {
                words[i].count++;
                for (int k = j; k < *word_count - 1; k++) {
                    words[k] = words[k + 1];
                }
                (*word_count)--;
                j--;
            }
        }
    }
}

void print_word_analysis(struct WordInfo words[], int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    printf("Total unique words: %d\n", word_count);
    printf("Word\t\tCount\tFirst Position\n");
    printf("----\t\t-----\t-------------\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s\t%d\t%d\n", words[i].word, words[i].count, words[i].first_position);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n')