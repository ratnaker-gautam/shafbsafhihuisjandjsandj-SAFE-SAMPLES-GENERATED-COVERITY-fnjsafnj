//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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
    char input[MAX_INPUT_LEN];
    WordCount words_stack[MAX_WORDS];
    WordCount *words_heap = NULL;
    int word_count = 0;
    int heap_allocated = 0;

    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }

    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    char *token = strtok(lower_input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int last_char = token_len - 1;
            while (last_char >= 0 && !is_word_char(token[last_char])) {
                token[last_char] = '\0';
                last_char--;
            }
            
            int first_char = 0;
            while (token[first_char] != '\0' && !is_word_char(token[first_char])) {
                first_char++;
            }
            
            if (token[first_char] != '\0') {
                char *clean_word = token + first_char;
                if (strlen(clean_word) > 0) {
                    int found_index = find_word(words_stack, word_count, clean_word);
                    if (found_index >= 0) {
                        words_stack[found_index].count++;
                    } else {
                        if (word_count < MAX_WORDS) {
                            words_stack[word_count].word = malloc(strlen(clean_word) + 1);
                            if (words_stack[word_count].word == NULL) {
                                printf("Memory allocation failed\n");
                                free(lower_input);
                                free_words(words_stack, word_count);
                                return 1;
                            }
                            strcpy(words_stack[word_count].word, clean_word);
                            words_stack[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }

    if (word_count == MAX_WORDS) {
        words_heap = malloc(MAX_WORDS * 2 * sizeof(WordCount));
        if (words_heap != NULL) {
            heap_allocated = 1;
            for (int i = 0; i < word_count; i++) {
                words_heap[i] = words_stack[i];
            }
        }
    }

    WordCount *words_to_sort = heap_allocated ? words_heap : words_stack;
    qsort(words_to_sort, word_count, sizeof(WordCount), compare_word_count);

    printf("\nWord frequency analysis:\n");
    printf("------------------------\n");
    for (int i = 0; i < word_count; i++) {
        printf("%-20s: %d\n", words_to_sort[i].word, words_to_sort[i].count);
    }

    free(lower_input);
    free_words(words_stack, word_count);
    if (heap_allocated) {
        free_words(words_heap, word_count);
        free(words_heap);
    }

    return 0;
}