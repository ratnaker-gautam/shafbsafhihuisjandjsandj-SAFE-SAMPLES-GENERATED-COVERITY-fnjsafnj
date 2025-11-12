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
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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

void cleanup_word_counts(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Input error\n");
        free(word_counts);
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        free(word_counts);
        return 0;
    }
    
    int word_count = 0;
    char *token = strtok(input, " \t\n\r");
    
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char cleaned_token[token_len + 1];
            int clean_index = 0;
            
            for (size_t i = 0; i < token_len; i++) {
                if (is_valid_char(token[i])) {
                    cleaned_token[clean_index++] = tolower((unsigned char)token[i]);
                }
            }
            cleaned_token[clean_index] = '\0';
            
            if (clean_index > 0) {
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(word_counts[i].word, cleaned_token) == 0) {
                        word_counts[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    char *word_copy = strdup_safe(cleaned_token);
                    if (word_copy != NULL) {
                        word_counts[word_count].word = word_copy;
                        word_counts[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    cleanup_word_counts(word_counts, word_count);
    free(word_counts);
    
    return 0;
}