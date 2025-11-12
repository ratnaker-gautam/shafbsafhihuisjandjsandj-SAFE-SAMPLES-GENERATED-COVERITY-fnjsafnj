//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
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
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower(str[i]);
    }
}

int is_word_char(char c) {
    return isalnum(c) || c == '_';
}

int extract_words(const char *text, char words[][32], int max_words) {
    if (text == NULL || max_words <= 0) return 0;
    
    int word_count = 0;
    size_t i = 0;
    size_t len = strlen(text);
    
    while (i < len && word_count < max_words) {
        while (i < len && !is_word_char(text[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(text[i])) i++;
        size_t word_len = i - start;
        
        if (word_len >= 32) word_len = 31;
        
        strncpy(words[word_count], &text[start], word_len);
        words[word_count][word_len] = '\0';
        to_lowercase(words[word_count]);
        word_count++;
    }
    
    return word_count;
}

int compare_words(const void *a, const void *b) {
    return strcmp((const char*)a, (const char*)b);
}

void count_word_frequencies(char words[][32], int word_count, 
                           char unique_words[][32], int frequencies[], int *unique_count) {
    if (word_count <= 0 || unique_count == NULL) return;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], unique_words[j]) == 0) {
                frequencies[j]++;
                found = 1;
                break;
            }
        }
        if (!found && *unique_count < MAX_WORDS) {
            strcpy(unique_words[*unique_count], words[i]);
            frequencies[*unique_count] = 1;
            (*unique_count)++;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][32];
    char unique_words[MAX_WORDS][32];
    int frequencies[MAX_WORDS];
    int word_count, unique_count;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    word_count = extract_words(input, words, MAX_WORDS);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(words[0]), compare_words);
    
    count_word_frequencies(words, word_count, unique_words, frequencies, &unique_count);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], frequencies[i]);
    }
    
    printf("\nTotal words: %d\n", word_count);
    printf("Unique words: %d\n", unique_count);
    
    return 0;
}