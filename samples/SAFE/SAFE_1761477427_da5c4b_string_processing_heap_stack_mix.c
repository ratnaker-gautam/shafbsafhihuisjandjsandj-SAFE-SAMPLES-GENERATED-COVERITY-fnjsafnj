//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordCount {
    char *word;
    int count;
};

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

int compare_words(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_input(const char *input, struct WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN - 1) return;
    
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

void cleanup_words(struct WordCount *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    for (size_t i = 0; i < strlen(input); i++) {
        if (!is_valid_char(input[i]) && !isspace((unsigned char)input[i])) {
            fprintf(stderr, "Invalid character in input\n");
            return 1;
        }
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_words);
        
        printf("Word frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}