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
    size_t i = 0;
    while (i < len && count < max_words) {
        while (i < len && isspace((unsigned char)str[i])) i++;
        if (i >= len) break;
        size_t start = i;
        while (i < len && !isspace((unsigned char)str[i])) i++;
        size_t word_len = i - start;
        if (word_len >= MAX_INPUT_LEN) return -1;
        strncpy(words[count], str + start, word_len);
        words[count][word_len] = '\0';
        count++;
    }
    return count;
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

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_INPUT_LEN];
    
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
    if (word_count <= 0) {
        fprintf(stderr, "Error splitting words\n");
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        to_lowercase(words[i]);
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_strings);
    
    printf("Sorted words (lowercase):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("Total words processed: %d\n", word_count);
    
    return 0;
}