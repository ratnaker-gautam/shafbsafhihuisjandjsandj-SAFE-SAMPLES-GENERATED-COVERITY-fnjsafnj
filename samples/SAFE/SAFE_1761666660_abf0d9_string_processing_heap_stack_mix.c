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
    const struct WordCount *wa = a;
    const struct WordCount *wb = b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words_stack[MAX_WORDS];
    struct WordCount *words_heap = NULL;
    int word_count = 0;
    int use_heap = 0;

    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }

    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }

    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int valid = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!is_word_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                int found_idx = find_word(words_stack, word_count, token);
                if (found_idx >= 0) {
                    words_stack[found_idx].count++;
                } else {
                    char *word_copy = strdup_safe(token);
                    if (word_copy == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(lower_input);
                        free_words(words_stack, word_count);
                        return 1;
                    }
                    if (word_count < MAX_WORDS) {
                        words_stack[word_count].word = word_copy;
                        words_stack[word_count].count = 1;
                        word_count++;
                    } else {
                        free(word_copy);
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }

    if (word_count > MAX_WORDS / 2) {
        use_heap = 1;
        words_heap = malloc(sizeof(struct WordCount) * word_count);
        if (words_heap == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(lower_input);
            free_words(words_stack, word_count);
            return 1;
        }
        for (int i = 0; i < word_count; i++) {
            words_heap[i] = words_stack[i];
        }
        qsort(words_heap, word_count, sizeof(struct WordCount), compare_word_count);
    } else {
        qsort(words_stack, word_count, sizeof(struct WordCount), compare_word_count);
    }

    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (use_heap) {
            printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
        } else {
            printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
        }
    }

    free(lower_input);
    if (use_heap) {
        free_words(words_heap, word_count);
        free(words_heap);
    } else {
        free_words(words_stack, word_count);
    }

    return 0;
}