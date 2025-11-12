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
    return isalnum(c) || c == '\'';
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
    struct WordCount *words = words_stack;
    int word_count = 0;
    int use_heap = 0;

    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Input error\n");
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

    if (input_len > 500) {
        words = malloc(MAX_WORDS * sizeof(struct WordCount));
        if (words == NULL) {
            printf("Memory allocation failed\n");
            return 1;
        }
        use_heap = 1;
    }

    char *text = to_lowercase(input);
    if (text == NULL) {
        printf("Memory allocation failed\n");
        if (use_heap) free(words);
        return 1;
    }

    char *token = strtok(text, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int start = 0;
            int end = token_len - 1;
            while (start < (int)token_len && !is_word_char(token[start])) start++;
            while (end >= 0 && !is_word_char(token[end])) end--;
            
            if (start <= end) {
                int word_len = end - start + 1;
                if (word_len > 0 && word_len <= 50) {
                    char clean_word[51];
                    strncpy(clean_word, token + start, word_len);
                    clean_word[word_len] = '\0';
                    
                    int found_idx = find_word(words, word_count, clean_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else {
                        char *word_copy = strdup_safe(clean_word);
                        if (word_copy != NULL) {
                            words[word_count].word = word_copy;
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }

    free(text);

    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);

    printf("Word frequencies:\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }

    free_words(words, word_count);
    if (use_heap) free(words);

    return 0;
}