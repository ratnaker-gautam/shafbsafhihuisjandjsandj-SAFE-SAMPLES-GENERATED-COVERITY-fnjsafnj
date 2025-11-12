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
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    WordCount *word_counts = malloc(MAX_WORDS * sizeof(WordCount));
    if (word_counts == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *token = strtok(input, " \t\n\r");
    
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        int valid = 1;
        for (size_t i = 0; i < token_len; i++) {
            if (!is_valid_char(token[i])) {
                valid = 0;
                break;
            }
        }
        
        if (valid && token_len > 0) {
            for (size_t i = 0; i < token_len; i++) {
                token[i] = tolower((unsigned char)token[i]);
            }
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(word_counts[i].word, token) == 0) {
                    word_counts[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = strdup_safe(token);
                if (word_copy != NULL) {
                    word_counts[word_count].word = word_copy;
                    word_counts[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    qsort(word_counts, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequencies (top 10):\n");
    int limit = word_count < 10 ? word_count : 10;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(word_counts[i].word);
    }
    free(word_counts);
    
    return 0;
}