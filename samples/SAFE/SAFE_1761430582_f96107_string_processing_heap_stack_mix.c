//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 64
#define MAX_WORDS 100

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

int process_input_string(const char *input, WordCount **word_counts, int *unique_count) {
    if (input == NULL || word_counts == NULL || unique_count == NULL) {
        return 0;
    }
    
    size_t len = strlen(input);
    if (len == 0 || len > 1024) {
        return 0;
    }
    
    char *buffer = malloc(len + 1);
    if (buffer == NULL) {
        return 0;
    }
    
    strncpy(buffer, input, len);
    buffer[len] = '\0';
    
    char *words[MAX_WORDS];
    int word_count = 0;
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                words[word_count] = malloc(token_len + 1);
                if (words[word_count] != NULL) {
                    strncpy(words[word_count], token, token_len);
                    words[word_count][token_len] = '\0';
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(buffer);
    
    if (word_count == 0) {
        *unique_count = 0;
        return 1;
    }
    
    *word_counts = malloc(word_count * sizeof(WordCount));
    if (*word_counts == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        return 0;
    }
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], (*word_counts)[j].word) == 0) {
                (*word_counts)[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && *unique_count < word_count) {
            strncpy((*word_counts)[*unique_count].word, words[i], MAX_WORD_LEN - 1);
            (*word_counts)[*unique_count].word[MAX_WORD_LEN - 1] = '\0';
            (*word_counts)[*unique_count].count = 1;
            (*unique_count)++;
        }
        
        free(words[i]);
    }
    
    return 1;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[1025];
    
    printf("Enter text (max 1024 characters): ");
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
    
    WordCount *word_counts = NULL;
    int unique_count = 0;
    
    if (!process_input_string(input, &word_counts, &unique_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (unique_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    qsort(word_counts, unique_count, sizeof(WordCount), compare_word_counts);
    
    printf("Word frequencies (alphabetical order):\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free(word_counts);
    return 0;
}