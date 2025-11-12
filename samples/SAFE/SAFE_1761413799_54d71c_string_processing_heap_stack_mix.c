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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    strcpy(new_str, str);
    return new_str;
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char temp[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    strcpy(temp, input);
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *token = strtok(temp, " \t\n\r.,;:!?\"'()[]{}");
    
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcasecmp(word_counts[i].word, token) == 0) {
                    word_counts[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = safe_strdup(token);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    for (int i = 0; i < word_count; i++) {
                        free(word_counts[i].word);
                    }
                    free(word_counts);
                    return 1;
                }
                
                for (char *p = word_copy; *p; p++) {
                    *p = tolower(*p);
                }
                
                word_counts[word_count].word = word_copy;
                word_counts[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?\"'()[]{}");
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_counts);
    
    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%-15s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}