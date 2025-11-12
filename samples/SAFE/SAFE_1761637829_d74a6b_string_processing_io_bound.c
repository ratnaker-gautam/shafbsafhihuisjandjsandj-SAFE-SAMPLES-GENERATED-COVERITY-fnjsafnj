//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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

void trim_whitespace(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    size_t start = 0;
    while (start < len && isspace((unsigned char)str[start])) start++;
    size_t end = len;
    while (end > start && isspace((unsigned char)str[end - 1])) end--;
    memmove(str, str + start, end - start);
    str[end - start] = '\0';
}

int split_words(const char *str, char words[][MAX_INPUT_LEN], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    int count = 0;
    size_t len = strlen(str);
    size_t start = 0;
    while (start < len && count < max_words) {
        while (start < len && isspace((unsigned char)str[start])) start++;
        if (start >= len) break;
        size_t end = start;
        while (end < len && !isspace((unsigned char)str[end])) end++;
        size_t word_len = end - start;
        if (word_len >= MAX_INPUT_LEN) return -1;
        strncpy(words[count], str + start, word_len);
        words[count][word_len] = '\0';
        count++;
        start = end;
    }
    return count;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int count_word_frequency(const char *word, char words[][MAX_INPUT_LEN], int word_count) {
    if (word == NULL || word_count <= 0) return 0;
    int count = 0;
    for (int i = 0; i < word_count; i++) {
        if (strcmp(word, words[i]) == 0) count++;
    }
    return count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    trim_whitespace(input);
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count < 0) {
        fprintf(stderr, "Input too long to process\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("Unique words: ");
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
            if (unique_count > 1) printf(", ");
            printf("%s", words[i]);
        }
    }
    printf("\n");
    
    printf("\nDetailed frequency:\n");
    for (int i = 0; i < word_count; i++) {
        int already_counted = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                already_counted = 1;
                break;
            }
        }
        if (!already_counted) {
            int freq = count_word_frequency(words[i], words, word_count);
            printf("%s: %d\n", words[i], freq);
        }
    }
    
    return 0;
}