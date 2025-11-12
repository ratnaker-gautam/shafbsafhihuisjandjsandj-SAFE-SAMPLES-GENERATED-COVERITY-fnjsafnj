//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

struct WordInfo {
    char *word;
    int count;
};

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str) + 1;
    if (len > MAX_INPUT_LEN) return NULL;
    char *new_str = malloc(len);
    if (new_str == NULL) return NULL;
    memcpy(new_str, str, len);
    return new_str;
}

int find_word_index(struct WordInfo *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void free_words(struct WordInfo *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo words_stack[MAX_WORDS];
    struct WordInfo *words_heap = NULL;
    int word_count = 0;
    int use_heap = 0;

    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    input[strcspn(input, "\n")] = '\0';

    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    char *token = strtok(input, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        if (strlen(token) == 0) {
            token = strtok(NULL, " \t");
            continue;
        }

        int found_index = find_word_index(words_stack, word_count, token);
        if (found_index == -1) {
            char *word_copy = safe_strdup(token);
            if (word_copy == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                free_words(words_stack, word_count);
                return 1;
            }

            if (word_count < MAX_WORDS / 2) {
                words_stack[word_count].word = word_copy;
                words_stack[word_count].count = 1;
            } else {
                if (!use_heap) {
                    words_heap = malloc(sizeof(struct WordInfo) * (MAX_WORDS - word_count));
                    if (words_heap == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(word_copy);
                        free_words(words_stack, word_count);
                        return 1;
                    }
                    use_heap = 1;
                }
                words_heap[word_count - MAX_WORDS / 2].word = word_copy;
                words_heap[word_count - MAX_WORDS / 2].count = 1;
            }
            word_count++;
        } else {
            if (found_index < MAX_WORDS / 2) {
                words_stack[found_index].count++;
            } else {
                words_heap[found_index - MAX_WORDS / 2].count++;
            }
        }

        token = strtok(NULL, " \t");
    }

    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        const char *word;
        int count;
        if (i < MAX_WORDS / 2) {
            word = words_stack[i].word;
            count = words_stack[i].count;
        } else {
            word = words_heap[i - MAX_WORDS / 2].word;
            count = words_heap[i - MAX_WORDS / 2].count;
        }
        printf("%s: %d\n", word, count);
    }

    free_words(words_stack, word_count < MAX_WORDS / 2 ? word_count : MAX_WORDS / 2);
    if (use_heap) {
        for (int i = 0; i < word_count - MAX_WORDS / 2; i++) {
            free(words_heap[i].word);
        }
        free(words_heap);
    }

    return 0;
}