//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
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

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void process_words(struct WordInfo *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_words);
    
    printf("Word frequencies (alphabetical order):\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].word != NULL) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordInfo words_stack[MAX_WORDS];
    struct WordInfo *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *input_copy = safe_strdup(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(input_copy, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words_stack[i].word, token) == 0) {
                    words_stack[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = safe_strdup(token);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(input_copy);
                    return 1;
                }
                words_stack[word_count].word = word_copy;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n");
    }
    
    free(input_copy);
    
    if (word_count > 0) {
        words_heap = malloc(word_count * sizeof(struct WordInfo));
        if (words_heap == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            for (int i = 0; i < word_count; i++) {
                free(words_stack[i].word);
            }
            return 1;
        }
        
        for (int i = 0; i < word_count; i++) {
            words_heap[i] = words_stack[i];
        }
        
        process_words(words_heap, word_count);
        
        for (int i = 0; i < word_count; i++) {
            free(words_heap[i].word);
        }
        free(words_heap);
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}