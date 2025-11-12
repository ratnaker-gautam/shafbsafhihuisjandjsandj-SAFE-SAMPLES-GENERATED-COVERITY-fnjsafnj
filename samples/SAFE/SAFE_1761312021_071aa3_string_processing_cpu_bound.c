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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

int extract_words(const char *text, char words[MAX_WORDS][64]) {
    if (text == NULL) return 0;
    int word_count = 0;
    size_t text_len = strlen(text);
    size_t word_start = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= text_len; i++) {
        if (i < text_len && is_word_char(text[i])) {
            if (!in_word) {
                word_start = i;
                in_word = 1;
            }
        } else {
            if (in_word) {
                size_t word_len = i - word_start;
                if (word_len < 64 && word_count < MAX_WORDS) {
                    strncpy(words[word_count], text + word_start, word_len);
                    words[word_count][word_len] = '\0';
                    to_lowercase(words[word_count]);
                    word_count++;
                }
                in_word = 0;
            }
        }
    }
    return word_count;
}

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

void count_word_frequencies(char words[MAX_WORDS][64], int word_count, 
                           char *unique_words[MAX_WORDS], int counts[MAX_WORDS], 
                           int *unique_count) {
    if (word_count <= 0 || word_count > MAX_WORDS) return;
    
    char *sorted_words[MAX_WORDS];
    for (int i = 0; i < word_count; i++) {
        sorted_words[i] = words[i];
    }
    qsort(sorted_words, word_count, sizeof(char*), compare_strings);
    
    *unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(sorted_words[i], sorted_words[i-1]) != 0) {
            unique_words[*unique_count] = sorted_words[i];
            counts[*unique_count] = 1;
            (*unique_count)++;
        } else {
            counts[(*unique_count)-1]++;
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    char *unique_words[MAX_WORDS];
    int counts[MAX_WORDS];
    int unique_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = extract_words(input, words);
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    count_word_frequencies(words, word_count, unique_words, counts, &unique_count);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", unique_words[i], counts[i]);
    }
    printf("Total unique words: %d\n", unique_count);
    printf("Total words: %d\n", word_count);
    
    return 0;
}