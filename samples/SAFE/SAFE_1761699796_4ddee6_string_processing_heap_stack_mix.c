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

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower(str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    WordCount words_stack[MAX_WORDS];
    int word_count = 0;
    char *word = strtok(lower_input, " \t\n\r");
    
    while (word != NULL && word_count < MAX_WORDS) {
        size_t word_len = strlen(word);
        while (word_len > 0 && !is_word_char(word[word_len - 1])) {
            word[word_len - 1] = '\0';
            word_len--;
        }
        
        while (word_len > 0 && !is_word_char(word[0])) {
            memmove(word, word + 1, word_len);
            word_len--;
        }
        
        if (word_len > 0) {
            int found_index = find_word(words_stack, word_count, word);
            if (found_index >= 0) {
                words_stack[found_index].count++;
            } else {
                char *word_copy = malloc(word_len + 1);
                if (word_copy == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(lower_input);
                    return 1;
                }
                strcpy(word_copy, word);
                words_stack[word_count].word = word_copy;
                words_stack[word_count].count = 1;
                word_count++;
            }
        }
        word = strtok(NULL, " \t\n\r");
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words_stack[i].word);
    }
    free(lower_input);
    
    return 0;
}