//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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
        if (!isprint(input[i]) && !IS_WHITESPACE(input[i])) {
            return 0;
        }
    }
    return 1;
}

int extract_words(const char *input, struct WordInfo words[], int *word_count) {
    if (!is_valid_input(input) || words == NULL || word_count == NULL) {
        return 0;
    }
    
    *word_count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t i = 0;
    int position = 0;
    
    while (i < input_len && *word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) break;
        
        size_t word_start = i;
        size_t word_len = 0;
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && !IS_TERMINATOR(input[i])) {
            if (word_len < MAX_WORD_LEN) {
                words[*word_count].word[word_len] = tolower(input[i]);
                word_len++;
            }
            i++;
        }
        
        if (word_len > 0) {
            words[*word_count].word[word_len] = '\0';
            words[*word_count].count = 1;
            words[*word_count].first_position = position;
            (*word_count)++;
            position++;
        }
        
        if (i < input_len && IS_TERMINATOR(input[i])) {
            i++;
        }
    }
    
    return 1;
}

int find_word_index(struct WordInfo words[], int word_count, const char *word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_word_frequency(struct WordInfo words[], int *word_count) {
    if (words == NULL || word_count == NULL || *word_count == 0) {
        return;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (words[i].count > 0) {
            int found_index = find_word_index(words, unique_count, words[i].word);
            if (found_index == -1) {
                if (unique_count < i) {
                    words[unique_count] = words[i];
                }
                unique_count++;
            } else {
                words[found_index].count++;
            }
        }
    }
    
    *word_count = unique_count;
}

int compare_word_frequency(const void *a, const void *b) {
    const struct WordInfo *word_a = (const struct WordInfo *)a;
    const struct WordInfo *word_b = (const struct WordInfo *)b;
    
    if (word_a->count != word_b->count) {
        return word_b->count - word_a->count;
    }
    return word_a->first_position - word_b->first_position;
}

int main(void) {
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
        printf("Invalid input detected.\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing input.\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input.\n");
        return 0;
    }
    
    process_word_frequency(words, &word_count);
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_word_frequency);
    
    printf("\nWord frequency analysis:\n");
    printf("%