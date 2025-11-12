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

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalpha((unsigned char)c);
}

char **extract_words(const char *text, int *word_count) {
    if (text == NULL || word_count == NULL) return NULL;
    
    char **words = malloc(MAX_WORDS * sizeof(char *));
    if (words == NULL) return NULL;
    
    char buffer[MAX_INPUT_LEN];
    if (strlen(text) >= MAX_INPUT_LEN - 1) {
        free(words);
        return NULL;
    }
    strcpy(buffer, text);
    
    *word_count = 0;
    char *token = strtok(buffer, " \t\n\r.,;:!?\"'()[]{}");
    
    while (token != NULL && *word_count < MAX_WORDS) {
        int valid = 1;
        for (size_t i = 0; token[i] != '\0'; i++) {
            if (!is_word_char(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && strlen(token) > 0) {
            char *word = to_lower_case(token);
            if (word != NULL) {
                words[*word_count] = word;
                (*word_count)++;
            }
        }
        
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    return words;
}

int find_word_in_array(struct WordCount *word_counts, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(word_counts[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_word_counts(struct WordCount *word_counts, int count) {
    for (int i = 0; i < count; i++) {
        free(word_counts[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    int word_count = 0;
    char **words = extract_words(input, &word_count);
    if (words == NULL) {
        fprintf(stderr, "Error processing words\n");
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
    
    struct WordCount *word_counts = malloc(word_count * sizeof(struct WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return 1;
    }
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int found_index = find_word_in_array(word_counts, unique_count, words[i]);
        if (found_index >= 0) {
            word_counts[found_index].count++;
        } else {
            word_counts[unique_count].word = malloc(strlen(words[i]) + 1);
            if (word_counts[unique_count].word == NULL) {
                free_word_counts(word_counts, unique_count);
                free(word_counts);
                for (int j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                free(words);
                return 1;
            }
            strcpy(word_counts[unique_count].word, words[i]);
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free_word_counts(word_counts, unique_count);
    free(word_counts);
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);
    
    return 0;
}