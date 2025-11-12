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

int split_words(const char *str, char words[][64], int max_words) {
    if (str == NULL || max_words <= 0) return 0;
    int count = 0;
    const char *p = str;
    while (*p && count < max_words) {
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        const char *start = p;
        while (*p && !isspace((unsigned char)*p)) p++;
        size_t word_len = p - start;
        if (word_len >= 64) word_len = 63;
        strncpy(words[count], start, word_len);
        words[count][word_len] = '\0';
        count++;
    }
    return count;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            word[i] = tolower((unsigned char)word[i]);
        }
    }
}

int compare_words(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    
    printf("Enter text to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    trim_whitespace(input);
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Input is empty after trimming\n");
        return 1;
    }
    
    int word_count = split_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        fprintf(stderr, "No words found in input\n");
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_words);
    
    printf("Processed words in alphabetical order:\n");
    for (int i = 0; i < word_count; i++) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    return 0;
}