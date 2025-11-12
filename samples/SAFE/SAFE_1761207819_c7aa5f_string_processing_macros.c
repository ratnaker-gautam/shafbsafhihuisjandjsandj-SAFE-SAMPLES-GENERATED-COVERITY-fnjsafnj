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
    return 1;
}

int extract_words(const char *input, struct WordInfo words[], int *word_count) {
    if (!is_valid_input(input)) return 0;
    
    int count = 0;
    size_t i = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            for (size_t j = 0; j < word_len; j++) {
                words[count].word[j] = tolower(input[start + j]);
            }
            words[count].word[word_len] = '\0';
            words[count].count = 1;
            words[count].first_position = count;
            
            int found_duplicate = 0;
            for (int k = 0; k < count; k++) {
                if (strcmp(words[k].word, words[count].word) == 0) {
                    words[k].count++;
                    found_duplicate = 1;
                    break;
                }
            }
            
            if (!found_duplicate) {
                count++;
            }
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    *word_count = count;
    return 1;
}

void print_word_stats(const struct WordInfo words[], int word_count) {
    if (word_count == 0) {
        printf("No words found.\n");
        return;
    }
    
    printf("Word statistics:\n");
    printf("Total unique words: %d\n", word_count);
    printf("\nWord frequency:\n");
    
    for (int i = 0; i < word_count; i++) {
        printf("%-15s: %d occurrence(s)\n", words[i].word, words[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: too long or empty.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    print_word_stats(words, word_count);
    
    return 0;
}