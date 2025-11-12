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
    if (len >= MAX_INPUT_LEN) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    strcpy(copy, s);
    return copy;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

char *to_lower_case(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len >= MAX_INPUT_LEN) return NULL;
    char *lower = malloc(len + 1);
    if (lower == NULL) return NULL;
    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower((unsigned char)s[i]);
    }
    lower[len] = '\0';
    return lower;
}

int find_word_index(struct WordCount *words, int count, const char *word) {
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

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words_stack[MAX_WORDS];
    struct WordCount *words_heap = NULL;
    int word_count = 0;
    int use_heap = 0;

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
        printf("Empty input\n");
        return 1;
    }

    if (input_len >= MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }

    char *lower_input = to_lower_case(input);
    if (lower_input == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    char *token = strtok(lower_input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            char *clean_token = malloc(token_len + 1);
            if (clean_token == NULL) {
                free(lower_input);
                free_words(words_heap, word_count);
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
                char *final_token = to_lower_case(clean_token);
                free(clean_token);
                if (final_token == NULL) {
                    free(lower_input);
                    free_words(words_heap, word_count);
                    printf("Memory allocation failed\n");
                    return 1;
                }

                int existing_index = find_word_index(words_stack, word_count, final_token);
                if (existing_index == -1) {
                    if (word_count < MAX_WORDS) {
                        if (word_count >= 50 && !use_heap) {
                            words_heap = malloc(sizeof(struct WordCount) * MAX_WORDS);
                            if (words_heap == NULL) {
                                free(final_token);
                                free(lower_input);
                                free_words(words_stack, word_count);
                                printf("Memory allocation failed\n");
                                return 1;
                            }
                            memcpy(words_heap, words_stack, sizeof(struct WordCount) * word_count);
                            use_heap = 1;
                        }

                        struct WordCount *current_words = use_heap ? words_heap : words_stack;
                        current_words[word_count].word = strdup_safe(final_token);
                        if (current_words[word_count].word == NULL) {
                            free(final_token);
                            free(lower_input);
                            free_words(current_words, word_count);
                            if (use_heap) free(words_heap);
                            printf("Memory allocation failed\n");
                            return 1;
                        }
                        current_words[word_count].count = 1;
                        word_count++;
                    }
                } else {
                    struct WordCount *current_words = use_heap ? words_heap : words_stack;
                    current_words[existing_index].count++;
                }
                free(final_token);
            } else {
                free(clean_token);
            }
        }
        token = strtok(NULL, " \t\n\r");
    }

    free(lower_input);

    printf("Word frequencies:\n");
    struct WordCount *current_words = use_heap ? words_heap : words_stack;
    for (int i = 0