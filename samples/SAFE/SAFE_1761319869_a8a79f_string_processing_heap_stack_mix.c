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
    char *dup = malloc(len + 1);
    if (dup == NULL) return NULL;
    strcpy(dup, s);
    return dup;
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
    const struct WordCount *wa = a;
    const struct WordCount *wb = b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    
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
    
    char *input_heap = strdup_safe(input);
    if (input_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *current = input_heap;
    while (*current != '\0' && word_count < MAX_WORDS) {
        while (*current != '\0' && !is_word_char(*current)) {
            current++;
        }
        
        if (*current == '\0') break;
        
        char *word_start = current;
        while (*current != '\0' && is_word_char(*current)) {
            current++;
        }
        
        char temp = *current;
        *current = '\0';
        
        char *lower_word = to_lowercase(word_start);
        if (lower_word == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(input_heap);
            return 1;
        }
        
        int existing_index = find_word(words_stack, word_count, lower_word);
        if (existing_index >= 0) {
            words_stack[existing_index].count++;
            free(lower_word);
        } else {
            char *word_copy = strdup_safe(lower_word);
            free(lower_word);
            if (word_copy == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free(input_heap);
                return 1;
            }
            words_stack[word_count].word = word_copy;
            words_stack[word_count].count = 1;
            word_count++;
        }
        
        *current = temp;
        if (*current != '\0') current++;
    }
    
    free(input_heap);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    struct WordCount *sorted_words = malloc(sizeof(struct WordCount) * word_count);
    if (sorted_words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free_words(words_stack, word_count);
        return 1;
    }
    
    memcpy(sorted_words, words_stack, sizeof(struct WordCount) * word_count);
    qsort(sorted_words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("Word frequencies (top 10):\n");
    int limit = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d\n", sorted_words[i].word, sorted_words[i].count);
    }
    
    free(sorted_words);
    free_words(words_stack, word_count);
    
    return 0;
}