//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha(c) || c == '\'';
}

char **split_words(const char *str, int *word_count) {
    if (str == NULL || word_count == NULL) return NULL;
    *word_count = 0;
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    const char *p = str;
    while (*p && *word_count < MAX_WORDS) {
        while (*p && !is_word_char(*p)) p++;
        if (!*p) break;
        
        const char *start = p;
        while (*p && is_word_char(*p)) p++;
        size_t word_len = p - start;
        
        if (word_len > 0) {
            char *word = malloc(word_len + 1);
            if (word == NULL) {
                for (int i = 0; i < *word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            strncpy(word, start, word_len);
            word[word_len] = '\0';
            words[*word_count] = word;
            (*word_count)++;
        }
    }
    return words;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wc1 = (const WordCount *)a;
    const WordCount *wc2 = (const WordCount *)b;
    return strcmp(wc1->word, wc2->word);
}

WordCount *count_words(char **words, int word_count, int *unique_count) {
    if (words == NULL || word_count <= 0 || unique_count == NULL) return NULL;
    *unique_count = 0;
    
    WordCount *counts = malloc(word_count * sizeof(WordCount));
    if (counts == NULL) return NULL;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], counts[j].word) == 0) {
                counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found) {
            counts[*unique_count].word = malloc(strlen(words[i]) + 1);
            if (counts[*unique_count].word == NULL) {
                for (int k = 0; k < *unique_count; k++) free(counts[k].word);
                free(counts);
                return NULL;
            }
            strcpy(counts[*unique_count].word, words[i]);
            counts[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
    
    qsort(counts, *unique_count, sizeof(WordCount), compare_words);
    return counts;
}

void cleanup_words(char **words, int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);
}

void cleanup_counts(WordCount *counts, int unique_count) {
    if (counts == NULL) return;
    for (int i = 0; i < unique_count; i++) {
        free(counts[i].word);
    }
    free(counts);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char **words = split_words(lower_input, &word_count);
    free(lower_input);
    
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No words found