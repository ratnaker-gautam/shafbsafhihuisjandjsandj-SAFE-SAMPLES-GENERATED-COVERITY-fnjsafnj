//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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
    return isalnum(c) || c == '\'';
}

int add_word(struct WordInfo *words, int *word_count, const char *word) {
    if (words == NULL || word_count == NULL || word == NULL) return 0;
    if (*word_count >= MAX_WORDS) return 0;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    
    words[*word_count].word = malloc(strlen(word) + 1);
    if (words[*word_count].word == NULL) return 0;
    strcpy(words[*word_count].word, word);
    words[*word_count].count = 1;
    (*word_count)++;
    return 1;
}

void process_string(const char *input, struct WordInfo *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    char *lower = to_lowercase(input);
    if (lower == NULL) return;
    
    char *copy = malloc(strlen(lower) + 1);
    if (copy == NULL) {
        free(lower);
        return;
    }
    strcpy(copy, lower);
    free(lower);
    
    char *token = strtok(copy, " \t\n\r");
    while (token != NULL && *word_count < MAX_WORDS) {
        size_t len = strlen(token);
        while (len > 0 && !is_word_char(token[len - 1])) {
            token[--len] = '\0';
        }
        while (*token && !is_word_char(*token)) {
            token++;
        }
        if (strlen(token) > 0) {
            add_word(words, word_count, token);
        }
        token = strtok(NULL, " \t\n\r");
    }
    free(copy);
}

void print_word_stats(struct WordInfo *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

void cleanup_words(struct WordInfo *words, int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_string(input, words, &word_count);
    print_word_stats(words, word_count);
    cleanup_words(words, word_count);
    
    return 0;
}