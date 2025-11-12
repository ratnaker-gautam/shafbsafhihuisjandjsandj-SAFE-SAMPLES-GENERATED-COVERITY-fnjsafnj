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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    memcpy(copy, s, len + 1);
    return copy;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_input(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN) return;
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && *word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int found = 0;
            for (int i = 0; i < *word_count; i++) {
                if (strcmp(words[i].word, token) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                char *word_copy = strdup_safe(token);
                if (word_copy != NULL) {
                    words[*word_count].word = word_copy;
                    words[*word_count].count = 1;
                    (*word_count)++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
}

void cleanup_words(WordCount *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    WordCount *words = malloc(MAX_WORDS * sizeof(WordCount));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        free(words);
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        free(words);
        return 1;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequencies (alphabetical order):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    cleanup_words(words, word_count);
    free(words);
    
    return 0;
}