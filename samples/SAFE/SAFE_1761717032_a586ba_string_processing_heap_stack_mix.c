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
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
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

void free_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *word_start = NULL;
    for (size_t i = 0; i <= input_len; i++) {
        if (is_word_char(lower_input[i]) && word_start == NULL) {
            word_start = &lower_input[i];
        } else if ((!is_word_char(lower_input[i]) || lower_input[i] == '\0') && word_start != NULL) {
            size_t word_len = &lower_input[i] - word_start;
            if (word_len > 0 && word_count < MAX_WORDS) {
                char temp_word[256];
                if (word_len >= sizeof(temp_word)) {
                    word_len = sizeof(temp_word) - 1;
                }
                strncpy(temp_word, word_start, word_len);
                temp_word[word_len] = '\0';
                
                int found_idx = find_word(words_stack, word_count, temp_word);
                if (found_idx >= 0) {
                    words_stack[found_idx].count++;
                } else {
                    char *heap_word = malloc(word_len + 1);
                    if (heap_word == NULL) {
                        free(lower_input);
                        free_words(words_stack, word_count);
                        fprintf(stderr, "Memory allocation failed\n");
                        return 1;
                    }
                    strncpy(heap_word, temp_word, word_len);
                    heap_word[word_len] = '\0';
                    words_stack[word_count].word = heap_word;
                    words_stack[word_count].count = 1;
                    word_count++;
                }
            }
            word_start = NULL;
        }
    }
    
    qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (descending order):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
    }
    
    free(lower_input);
    free_words(words_stack, word_count);
    
    return 0;
}