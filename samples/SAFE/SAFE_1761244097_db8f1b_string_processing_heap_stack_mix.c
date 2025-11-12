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
    return isalnum((unsigned char)c) || c == ' ' || c == '\t' || c == '\n';
}

char *sanitize_input(const char *input) {
    if (input == NULL) return NULL;
    
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        if (!is_valid_char(input[i])) return NULL;
    }
    
    char *clean = malloc(len + 1);
    if (clean == NULL) return NULL;
    
    strcpy(clean, input);
    return clean;
}

char **split_words(const char *text, int *word_count) {
    if (text == NULL || word_count == NULL) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    char buffer[MAX_INPUT_LEN];
    strcpy(buffer, text);
    
    *word_count = 0;
    char *token = strtok(buffer, " \t\n");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            words[*word_count] = malloc(strlen(token) + 1);
            if (words[*word_count] == NULL) {
                for (int i = 0; i < *word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            strcpy(words[*word_count], token);
            (*word_count)++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    return words;
}

WordCount *count_word_frequencies(char **words, int word_count, int *unique_count) {
    if (words == NULL || word_count <= 0 || unique_count == NULL) return NULL;
    
    WordCount *freqs = malloc(word_count * sizeof(WordCount));
    if (freqs == NULL) return NULL;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], freqs[j].word) == 0) {
                freqs[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            freqs[*unique_count].word = malloc(strlen(words[i]) + 1);
            if (freqs[*unique_count].word == NULL) {
                for (int k = 0; k < *unique_count; k++) free(freqs[k].word);
                free(freqs);
                return NULL;
            }
            strcpy(freqs[*unique_count].word, words[i]);
            freqs[*unique_count].count = 1;
            (*unique_count)++;
        }
    }
    
    return freqs;
}

void cleanup_words(char **words, int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) free(words[i]);
    free(words);
}

void cleanup_frequencies(WordCount *freqs, int unique_count) {
    if (freqs == NULL) return;
    for (int i = 0; i < unique_count; i++) free(freqs[i].word);
    free(freqs);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    char *clean_input = sanitize_input(input);
    if (clean_input == NULL) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    int word_count = 0;
    char **words = split_words(clean_input, &word_count);
    free(clean_input);
    
    if (words == NULL || word_count == 0) {
        fprintf(stderr, "No valid words found\n");
        if (words != NULL) cleanup_words(words, word_count);
        return 1;
    }
    
    int unique_count = 0;
    WordCount *frequencies = count_word_frequencies(words, word_count, &unique_count);
    
    if (frequencies == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        cleanup_words(words, word_count);
        return 1;
    }
    
    printf("Word frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", frequencies[i].word, frequencies[i].count);
    }
    
    cleanup_words(words, word_count);
    cleanup_frequencies(frequencies, unique_count);
    
    return 0;
}