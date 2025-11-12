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
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '\'' || c == '-';
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
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words_stack[MAX_WORDS];
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *text_ptr = lower_input;
    while (*text_ptr && word_count < MAX_WORDS) {
        while (*text_ptr && !is_word_char(*text_ptr)) {
            text_ptr++;
        }
        
        if (!*text_ptr) break;
        
        char *word_start = text_ptr;
        while (*text_ptr && is_word_char(*text_ptr)) {
            text_ptr++;
        }
        
        size_t word_len = text_ptr - word_start;
        if (word_len == 0) continue;
        
        char *word = malloc(word_len + 1);
        if (word == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(lower_input);
            return 1;
        }
        
        strncpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        int existing_index = find_word(words_stack, word_count, word);
        if (existing_index >= 0) {
            words_stack[existing_index].count++;
            free(word);
        } else {
            if (word_count < MAX_WORDS) {
                words_stack[word_count].word = word;
                words_stack[word_count].count = 1;
                word_count++;
            } else {
                free(word);
            }
        }
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top 10):\n");
    int limit = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words_stack[i].word);
    }
    free(lower_input);
    
    return 0;
}