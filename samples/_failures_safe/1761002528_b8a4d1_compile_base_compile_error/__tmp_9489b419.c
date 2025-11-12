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
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
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

    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char *clean_token = malloc(token_len + 1);
            if (clean_token == NULL) {
                free(lower_input);
                free_words(words_stack, word_count);
                if (heap_allocated) free(words_heap);
                printf("Memory allocation failed\n");
                return 1;
            }

            int clean_index = 0;
            for (size_t i = 0; i < token_len; i++) {
                if (is_word_char(token[i])) {
                    clean_token[clean_index++] = token[i];
                }
            }
            clean_token[clean_index] = '\0';

            if (clean_index > 0) {
                int found_index = find_word(words_stack, word_count, clean_token);
                if (found_index >= 0) {
                    words_stack[found_index].count++;
                    free(clean_token);
                } else {
                    if (word_count < MAX_WORDS) {
                        words_stack[word_count].word = clean_token;
                        words_stack[word_count].count = 1;
                        word_count++;
                    } else {
                        free(clean_token);
                    }
                }
            } else {
                free(clean_token);
            }
        }
        token = strtok(NULL, " ");
    }

    free(lower_input);

    if (word_count > MAX_WORDS / 2) {
        words_heap = malloc(word_count * sizeof(WordCount));
        if (words_heap != NULL) {
            heap_allocated = 1;
            for (int i = 0; i < word_count; i++) {
                words_heap[i] = words_stack[i];
            }
            qsort(words_heap, word_count, sizeof(WordCount), compare_word_count);
            printf("Word frequencies (sorted by count):\n");
            for (int i = 0; i < word_count; i++) {
                printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
            }
            free(words_heap);
        } else {
            qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
            printf("Word frequencies (sorted by count):\n");
            for (int i = 0; i < word_count; i++) {
                printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
            }
        }
    } else {
        qsort(words_stack, word_count, sizeof(WordCount), compare_word_count);
        printf("Word frequencies (sorted by count):\n");
        for (int i = 0; i < word_count