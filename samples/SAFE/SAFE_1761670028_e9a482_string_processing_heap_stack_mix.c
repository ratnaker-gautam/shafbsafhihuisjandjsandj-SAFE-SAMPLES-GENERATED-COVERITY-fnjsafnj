//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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

int add_word(struct WordInfo *words, int *count, const char *word) {
    if (*count >= MAX_WORDS) return 0;
    for (int i = 0; i < *count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return 1;
        }
    }
    words[*count].word = malloc(strlen(word) + 1);
    if (words[*count].word == NULL) return 0;
    strcpy(words[*count].word, word);
    words[*count].count = 1;
    (*count)++;
    return 1;
}

void process_string(const char *input, struct WordInfo *words, int *word_count) {
    if (!is_valid_input(input)) return;
    char *lower = to_lowercase(input);
    if (lower == NULL) return;
    
    char buffer[MAX_INPUT_LEN];
    strcpy(buffer, lower);
    free(lower);
    
    char *ptr = buffer;
    while (*ptr) {
        while (*ptr && !is_word_char(*ptr)) ptr++;
        if (!*ptr) break;
        
        char *start = ptr;
        while (*ptr && is_word_char(*ptr)) ptr++;
        char temp = *ptr;
        *ptr = '\0';
        
        if (!add_word(words, word_count, start)) break;
        
        *ptr = temp;
        if (temp) ptr++;
    }
}

void print_results(const struct WordInfo *words, int count) {
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

void cleanup_words(struct WordInfo *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
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
    
    process_string(input, words, &word_count);
    print_results(words, word_count);
    cleanup_words(words, word_count);
    
    return 0;
}