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
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len >= MAX_INPUT_LEN) return NULL;
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    memcpy(new_str, str, len + 1);
    return new_str;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void cleanup_wordinfo(struct WordInfo *words, int count) {
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordInfo words_stack[MAX_WORDS];
    struct WordInfo *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *input_copy = safe_strdup(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words_stack[word_count].word = safe_strdup(token);
            if (words_stack[word_count].word == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                cleanup_wordinfo(words_stack, word_count);
                free(input_copy);
                return 1;
            }
            words_stack[word_count].count = 1;
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    if (word_count == 0) {
        printf("No words found\n");
        return 0;
    }
    
    words_heap = malloc(sizeof(struct WordInfo) * word_count);
    if (words_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        cleanup_wordinfo(words_stack, word_count);
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        words_heap[i].word = words_stack[i].word;
        words_heap[i].count = words_stack[i].count;
    }
    
    qsort(words_heap, word_count, sizeof(struct WordInfo), compare_words);
    
    printf("Sorted unique words:\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words_heap[i].word, words_heap[i-1].word) != 0) {
            printf("%s\n", words_heap[i].word);
        }
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words_heap[i].word);
    }
    free(words_heap);
    
    return 0;
}