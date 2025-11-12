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

char *to_lower_case(const char *str) {
    if (str == NULL) return NULL;
    
    size_t len = strlen(str);
    if (len == 0) return NULL;
    
    char *result = malloc(len + 1);
    if (result == NULL) return NULL;
    
    for (size_t i = 0; i < len; i++) {
        result[i] = tolower((unsigned char)str[i]);
    }
    result[len] = '\0';
    return result;
}

int find_word_index(WordCount *words, int count, const char *word) {
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

void cleanup_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words_stack[MAX_WORDS];
    WordCount *words_heap = NULL;
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    if (input_len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    words_heap = malloc(MAX_WORDS * sizeof(WordCount));
    if (words_heap == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }
    
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
        
        if (valid && token_len > 0 && token_len <= MAX_WORD_LEN) {
            char *lower_word = to_lower_case(token);
            if (lower_word == NULL) {
                token = strtok(NULL, " \t\n\r");
                continue;
            }
            
            int existing_index = find_word_index(words_heap, word_count, lower_word);
            
            if (existing_index >= 0) {
                words_heap[existing_index].count++;
                free(lower_word);
            } else {
                words_heap[word_count].word = lower_word;
                words_heap[word_count].count = 1;
                word_count++;
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    for (int i = 0; i < word_count && i < MAX_WORDS; i++) {
        words_stack[i] = words_heap[i];
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
    int display_count = word_count > 10 ? 10 : word_count;
    
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    cleanup_words(words_heap, word_count);
    free(words_heap);
    
    return 0;
}