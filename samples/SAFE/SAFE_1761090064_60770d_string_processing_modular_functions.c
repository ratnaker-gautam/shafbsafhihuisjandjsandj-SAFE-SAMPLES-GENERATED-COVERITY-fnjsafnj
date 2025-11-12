//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

struct WordCount {
    char word[MAX_WORD_LEN];
    int count;
};

int is_valid_input(const char* input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
    }
    return 1;
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!is_valid_input(input)) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha(input[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= MAX_WORD_LEN || *word_count >= MAX_WORDS) {
                    return 0;
                }
                strncpy(words[*word_count], input + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
        }
    }
    return 1;
}

void to_lowercase(char* str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* word_counts, int* unique_count) {
    if (words == NULL || word_counts == NULL || unique_count == NULL) return 0;
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LEN];
        strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
        temp_word[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp_word);
        
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(word_counts[j].word, temp_word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (*unique_count >= MAX_WORDS) return 0;
            strncpy(word_counts[*unique_count].word, temp_word, MAX_WORD_LEN - 1);
            word_counts[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            word_counts[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
    return 1;
}

void print_word_counts(const struct WordCount* word_counts, int unique_count) {
    if (word_counts == NULL || unique_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount word_counts[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    if (!extract_words(input, words, &word_count)) {
        printf("Error processing words\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    if (!count_unique_words(words, word_count, word_counts, &unique_count)) {
        printf("Error counting unique words\n");
        return 1;
    }
    
    print_word_counts(word_counts, unique_count);
    
    return 0;
}