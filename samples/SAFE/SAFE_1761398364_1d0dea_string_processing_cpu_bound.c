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

void normalize_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    size_t write_idx = 0;
    for (size_t i = 0; i < len; i++) {
        if (isalnum((unsigned char)word[i])) {
            word[write_idx++] = tolower((unsigned char)word[i]);
        }
    }
    word[write_idx] = '\0';
}

int compare_words(const void *a, const void *b) {
    const char *word1 = *(const char **)a;
    const char *word2 = *(const char **)b;
    return strcmp(word1, word2);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char temp[MAX_INPUT_LEN];
    char *words[MAX_WORDS];
    int word_count = 0;
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char *word = malloc(token_len + 1);
            if (word == NULL) {
                for (int i = 0; i < word_count; i++) free(words[i]);
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
            strcpy(word, token);
            normalize_word(word);
            if (strlen(word) > 0) {
                words[word_count++] = word;
            } else {
                free(word);
            }
        }
        token = strtok(NULL, " \t");
    }
    
    if (word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        return 1;
    }
    
    qsort(words, word_count, sizeof(char *), compare_words);
    
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
        free(words[i]);
    }
    
    return 0;
}