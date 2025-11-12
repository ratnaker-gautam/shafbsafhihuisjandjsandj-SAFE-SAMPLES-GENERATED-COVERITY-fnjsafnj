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
    if (len > MAX_INPUT_LEN) return NULL;
    char *new = malloc(len + 1);
    if (new == NULL) return NULL;
    strcpy(new, s);
    return new;
}

char *to_lowercase(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len > MAX_INPUT_LEN) return NULL;
    char *lower = malloc(len + 1);
    if (lower == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower(s[i]);
    }
    lower[len] = '\0';
    return lower;
}

int is_word_char(char c) {
    return isalpha(c) || c == '\'';
}

int find_word(struct WordCount *words, int count, const char *word) {
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

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    struct WordCount words_stack[MAX_WORDS];
    struct WordCount *words_heap = malloc(MAX_WORDS * sizeof(struct WordCount));
    if (words_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_count = 0;
    char *input_copy = strdup_safe(input);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(words_heap);
        return 1;
    }
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        char *clean_token = malloc(strlen(token) + 1);
        if (clean_token == NULL) {
            free(input_copy);
            free_words(words_heap, word_count);
            free(words_heap);
            return 1;
        }
        
        int clean_len = 0;
        for (int i = 0; token[i] != '\0'; i++) {
            if (is_word_char(token[i])) {
                clean_token[clean_len++] = token[i];
            }
        }
        clean_token[clean_len] = '\0';
        
        if (clean_len > 0) {
            char *lower_word = to_lowercase(clean_token);
            if (lower_word == NULL) {
                free(clean_token);
                free(input_copy);
                free_words(words_heap, word_count);
                free(words_heap);
                return 1;
            }
            
            int existing_index = find_word(words_heap, word_count, lower_word);
            if (existing_index >= 0) {
                words_heap[existing_index].count++;
                free(lower_word);
            } else {
                words_heap[word_count].word = lower_word;
                words_heap[word_count].count = 1;
                word_count++;
            }
        }
        
        free(clean_token);
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    for (int i = 0; i < word_count && i < MAX_WORDS; i++) {
        words_stack[i] = words_heap[i];
    }
    
    qsort(words_stack, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    free_words(words_heap, word_count);
    free(words_heap);
    
    return 0;
}