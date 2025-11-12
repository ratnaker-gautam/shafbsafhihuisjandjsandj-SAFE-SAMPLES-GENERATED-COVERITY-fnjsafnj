//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    char *input_copy = malloc(input_len + 1);
    if (input_copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    strcpy(input_copy, input);
    
    char *token = strtok(input_copy, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            int valid = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            
            if (valid) {
                char *lower_word = to_lower_case(token);
                if (lower_word != NULL) {
                    int existing_index = find_word_index(words_stack, word_count, lower_word);
                    
                    if (existing_index >= 0) {
                        words_stack[existing_index].count++;
                        free(lower_word);
                    } else {
                        words_stack[word_count].word = lower_word;
                        words_stack[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    free(input_copy);
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    WordCount *sorted_words = malloc(sizeof(WordCount) * word_count);
    if (sorted_words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        for (int i = 0; i < word_count; i++) {
            free(words_stack[i].word);
        }
        return 1;
    }
    
    memcpy(sorted_words, words_stack, sizeof(WordCount) * word_count);
    qsort(sorted_words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top 10):\n");
    int display_count = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < display_count; i++) {
        printf("%s: %d\n", sorted_words[i].word, sorted_words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(sorted_words[i].word);
    }
    free(sorted_words);
    
    return 0;
}