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

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

char *safe_strdup(const char *src) {
    if (src == NULL) return NULL;
    size_t len = strlen(src);
    if (len == 0) return NULL;
    char *dest = malloc(len + 1);
    if (dest == NULL) return NULL;
    strcpy(dest, src);
    return dest;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void free_word_array(struct WordInfo *words, int count) {
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

    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
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
        return 0;
    }

    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }

    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int valid = 1;
            for (size_t i = 0; i < token_len; i++) {
                if (!is_valid_char(token[i])) {
                    valid = 0;
                    break;
                }
            }
            if (valid) {
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words_stack[i].word, token) == 0) {
                        words_stack[i].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    if (word_count < MAX_WORDS) {
                        words_stack[word_count].word = safe_strdup(token);
                        if (words_stack[word_count].word == NULL) {
                            fprintf(stderr, "Memory allocation failed\n");
                            free_word_array(words_stack, word_count);
                            return 1;
                        }
                        words_stack[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }

    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }

    if (word_count > MAX_WORDS / 2) {
        use_heap = 1;
        words_heap = malloc(word_count * sizeof(struct WordInfo));
        if (words_heap == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free_word_array(words_stack, word_count);
            return 1;
        }
        for (int i = 0; i < word_count; i++) {
            words_heap[i].word = words_stack[i].word;
            words_heap[i].count = words_stack[i].count;
            words_stack[i].word = NULL;
        }
    }

    if (use_heap) {
        qsort(words_heap, word_count, sizeof(struct WordInfo), compare_words);
        printf("Word frequency (sorted alphabetically):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
        }
        free_word_array(words_heap, word_count);
        free(words_heap);
    } else {
        qsort(words_stack, word_count, sizeof(struct WordInfo), compare_words);
        printf("Word frequency (sorted alphabetically):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
        }
        free_word_array(words_stack, word_count);
    }

    return 0;
}