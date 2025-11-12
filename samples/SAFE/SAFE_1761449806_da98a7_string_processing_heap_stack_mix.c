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
    char *dup = malloc(len + 1);
    if (dup == NULL) return NULL;
    strcpy(dup, s);
    return dup;
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
    struct WordCount words[MAX_WORDS];
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
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    char *token = strtok(lower_input, " \t\n\r");
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
                int found_idx = find_word(words, word_count, token);
                if (found_idx >= 0) {
                    words[found_idx].count++;
                } else {
                    char *word_copy = strdup_safe(token);
                    if (word_copy == NULL) {
                        fprintf(stderr, "Memory allocation failed\n");
                        free(lower_input);
                        free_words(words, word_count);
                        return 1;
                    }
                    words[word_count].word = word_copy;
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    free(lower_input);
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("Word frequencies (top 10):\n");
    int limit = word_count > 10 ? 10 : word_count;
    for (int i = 0; i < limit; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    free_words(words, word_count);
    return 0;
}