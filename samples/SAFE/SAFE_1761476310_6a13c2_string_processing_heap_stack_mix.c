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

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len == 0) return NULL;
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    strncpy(new_str, str, len);
    new_str[len] = '\0';
    return new_str;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void cleanup_word_array(struct WordInfo *words, int count) {
    for (int i = 0; i < count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words_stack[MAX_WORDS];
    struct WordInfo *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *token = strtok(input, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words_stack[i].word, token) == 0) {
                    words_stack[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = safe_strdup(token);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    cleanup_word_array(words_stack, word_count);
                    return 1;
                }
                words_stack[word_count].word = word_copy;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    words_heap = malloc(sizeof(struct WordInfo) * word_count);
    if (words_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        cleanup_word_array(words_stack, word_count);
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        words_heap[i].word = words_stack[i].word;
        words_heap[i].count = words_stack[i].count;
        words_stack[i].word = NULL;
    }
    
    qsort(words_heap, word_count, sizeof(struct WordInfo), compare_words);
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
    }
    
    cleanup_word_array(words_heap, word_count);
    free(words_heap);
    
    return 0;
}