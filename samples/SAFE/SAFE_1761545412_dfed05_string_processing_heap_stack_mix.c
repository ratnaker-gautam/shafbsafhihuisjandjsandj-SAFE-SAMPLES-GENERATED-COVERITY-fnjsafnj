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

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *copy = malloc(len + 1);
    if (copy != NULL) {
        memcpy(copy, s, len + 1);
    }
    return copy;
}

char *to_lowercase(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    char *lower = malloc(len + 1);
    if (lower == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower((unsigned char)s[i]);
    }
    lower[len] = '\0';
    return lower;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(struct WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
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
    
    struct WordCount *word_counts = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *input_copy = strdup_safe(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(word_counts);
        return 1;
    }
    
    char *token = strtok(input_copy, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char *clean_token = malloc(token_len + 1);
            if (clean_token == NULL) {
                free(input_copy);
                free_words(word_counts, word_count);
                free(word_counts);
                return 1;
            }
            
            int clean_index = 0;
            for (size_t i = 0; i < token_len; i++) {
                if (is_word_char(token[i])) {
                    clean_token[clean_index++] = token[i];
                }
            }
            clean_token[clean_index] = '\0';
            
            if (clean_index > 0) {
                char *lower_token = to_lowercase(clean_token);
                if (lower_token != NULL) {
                    int existing_index = find_word_index(word_counts, word_count, lower_token);
                    if (existing_index >= 0) {
                        word_counts[existing_index].count++;
                    } else {
                        word_counts[word_count].word = strdup_safe(lower_token);
                        if (word_counts[word_count].word != NULL) {
                            word_counts[word_count].count = 1;
                            word_count++;
                        }
                    }
                    free(lower_token);
                }
            }
            free(clean_token);
        }
        token = strtok(NULL, " \t");
    }
    
    free(input_copy);
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    free_words(word_counts, word_count);
    free(word_counts);
    
    return 0;
}