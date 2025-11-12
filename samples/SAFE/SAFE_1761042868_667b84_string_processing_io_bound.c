//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int split_words(const char *str, char words[][64], int *word_count) {
    if (!is_valid_input(str)) return 0;
    *word_count = 0;
    size_t len = strlen(str);
    int in_word = 0;
    size_t word_start = 0;
    
    for (size_t i = 0; i <= len; i++) {
        if (i < len && !isspace((unsigned char)str[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len >= 64 || *word_count >= MAX_WORDS) return 0;
                strncpy(words[*word_count], str + word_start, word_len);
                words[*word_count][word_len] = '\0';
                (*word_count)++;
                in_word = 0;
            }
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

int compare_strings(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int process_string(const char *input) {
    if (!is_valid_input(input)) return 0;
    
    char words[MAX_WORDS][64];
    int word_count = 0;
    
    if (!split_words(input, words, &word_count)) return 0;
    if (word_count == 0) return 1;
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, 64, compare_strings);
    
    printf("Sorted words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nWord count: %d\n", word_count);
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i-1]) != 0) {
            unique_count++;
        }
    }
    
    printf("Unique words: %d\n", unique_count);
    return 1;
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to process (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!process_string(input)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    return 0;
}