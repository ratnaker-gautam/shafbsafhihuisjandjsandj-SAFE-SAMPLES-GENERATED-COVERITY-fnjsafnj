//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

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
    strcpy(copy, s);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_input(const char *input, WordCount *word_counts, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (strlen(input) >= MAX_INPUT_LEN - 1) return;
    strcpy(buffer, input);
    
    char *words[MAX_WORDS];
    int words_found = 0;
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && words_found < MAX_WORDS) {
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
                words[words_found] = strdup_safe(token);
                if (words[words_found] != NULL) {
                    words_found++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    for (int i = 0; i < words_found; i++) {
        int found = 0;
        for (int j = 0; j < *word_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        if (!found && *word_count < MAX_WORDS) {
            word_counts[*word_count].word = strdup_safe(words[i]);
            if (word_counts[*word_count].word != NULL) {
                word_counts[*word_count].count = 1;
                (*word_count)++;
            }
        }
        free(words[i]);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    process_input(input, word_counts, &word_count);
    
    if (word_count > 0) {
        qsort(word_counts, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
        }
        
        for (int i = 0; i < word_count; i++) {
            free(word_counts[i].word);
        }
    } else {
        printf("No valid words found\n");
    }
    
    return 0;
}