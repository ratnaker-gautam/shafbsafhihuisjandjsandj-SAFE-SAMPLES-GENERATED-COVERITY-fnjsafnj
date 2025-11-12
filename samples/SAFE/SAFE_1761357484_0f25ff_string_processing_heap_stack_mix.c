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
            char *lower_word = to_lower_case(token);
            if (lower_word != NULL) {
                words[*word_count] = lower_word;
                (*word_count)++;
            }
        }
        
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    return words;
}

int compare_words(const void *a, const void *b) {
    const struct WordCount *wc1 = (const struct WordCount *)a;
    const struct WordCount *wc2 = (const struct WordCount *)b;
    return strcmp(wc1->word, wc2->word);
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
    
    int word_count;
    char **words = extract_words(input, &word_count);
    
    if (words == NULL || word_count == 0) {
        printf("No valid words found\n");
        if (words != NULL) free(words);
        return 1;
    }
    
    struct WordCount *word_counts = malloc(word_count * sizeof(struct WordCount));
    if (word_counts == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            word_counts[unique_count].word = words[i];
            word_counts[unique_count].count = 1;
            unique_count++;
        } else {
            free(words[i]);
        }
    }
    
    qsort(word_counts, unique_count, sizeof(struct WordCount), compare_words);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < unique_count; i++) {
        free(word_counts[i].word);
    }
    
    free(word_counts);
    free(words);
    
    return 0;
}