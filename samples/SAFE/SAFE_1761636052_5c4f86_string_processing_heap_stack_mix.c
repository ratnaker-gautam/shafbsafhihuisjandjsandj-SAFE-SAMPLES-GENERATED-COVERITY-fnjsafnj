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

char **tokenize_string(const char *input, int *word_count) {
    if (input == NULL || word_count == NULL) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    char *copy = malloc(strlen(input) + 1);
    if (copy == NULL) {
        free(words);
        return NULL;
    }
    strcpy(copy, input);
    
    *word_count = 0;
    char *token = strtok(copy, " \t\n\r");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        int valid = 1;
        for (int i = 0; token[i] != '\0'; i++) {
            if (!is_valid_char(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) > 0) {
            words[*word_count] = malloc(strlen(token) + 1);
            if (words[*word_count] != NULL) {
                strcpy(words[*word_count], token);
                (*word_count)++;
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(copy);
    return words;
}

WordCount *count_word_frequencies(char **words, int word_count, int *unique_count) {
    if (words == NULL || unique_count == NULL) return NULL;
    
    WordCount *freqs = malloc(MAX_WORDS * sizeof(WordCount));
    if (freqs == NULL) return NULL;
    
    *unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (words[i] == NULL) continue;
        
        int found = 0;
        for (int j = 0; j < *unique_count; j++) {
            if (strcmp(words[i], freqs[j].word) == 0) {
                freqs[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && *unique_count < MAX_WORDS) {
            freqs[*unique_count].word = malloc(strlen(words[i]) + 1);
            if (freqs[*unique_count].word != NULL) {
                strcpy(freqs[*unique_count].word, words[i]);
                freqs[*unique_count].count = 1;
                (*unique_count)++;
            }
        }
    }
    
    return freqs;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    to_lowercase(input);
    
    int word_count = 0;
    char **words = tokenize_string(input, &word_count);
    
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    int unique_count = 0;
    WordCount *frequencies = count_word_frequencies(words, word_count, &unique_count);
    
    if (frequencies == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", frequencies[i].word, frequencies[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);
    
    for (int i = 0; i < unique_count; i++) {
        free(frequencies[i].word);
    }
    free(frequencies);
    
    return 0;
}