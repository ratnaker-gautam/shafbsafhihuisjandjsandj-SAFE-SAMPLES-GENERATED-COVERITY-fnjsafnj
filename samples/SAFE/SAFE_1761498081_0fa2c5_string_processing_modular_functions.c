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

int is_valid_input(const char* str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int extract_words(const char* input, char words[][MAX_WORD_LEN], int* word_count) {
    if (!is_valid_input(input)) return 0;
    if (words == NULL || word_count == NULL) return 0;
    
    *word_count = 0;
    size_t input_len = strlen(input);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
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
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_unique_words(char words[][MAX_WORD_LEN], int word_count, struct WordCount* unique_words, int* unique_count) {
    if (words == NULL || unique_words == NULL || unique_count == NULL) return 0;
    if (word_count <= 0 || word_count > MAX_WORDS) return 0;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char temp_word[MAX_WORD_LEN];
        strncpy(temp_word, words[i], MAX_WORD_LEN - 1);
        temp_word[MAX_WORD_LEN - 1] = '\0';
        to_lowercase(temp_word);
        
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(unique_words[j].word, temp_word) == 0) {
                unique_words[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (*unique_count >= MAX_WORDS) return 0;
            strncpy(unique_words[*unique_count].word, temp_word, MAX_WORD_LEN - 1);
            unique_words[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            unique_words[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
    return 1;
}

void print_word_counts(const struct WordCount* unique_words, int unique_count) {
    if (unique_words == NULL || unique_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i].word, unique_words[i].count);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    struct WordCount unique_words[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;
    
    printf("Enter text for word frequency analysis: ");
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
    
    if (!count_unique_words(words, word_count, unique_words, &unique_count)) {
        printf("Error counting unique words\n");
        return 1;
    }
    
    print_word_counts(unique_words, unique_count);
    
    return 0;
}