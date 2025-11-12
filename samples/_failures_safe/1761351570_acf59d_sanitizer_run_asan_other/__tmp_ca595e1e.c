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
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *safe_strdup(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    char *new_str = malloc(len + 1);
    if (new_str == NULL) return NULL;
    memcpy(new_str, str, len + 1);
    return new_str;
}

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

void process_string(const char *input, struct WordInfo **words, int *word_count) {
    char buffer[MAX_INPUT_LEN + 1];
    char *word;
    char *saveptr;
    
    if (!is_valid_input(input)) return;
    
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    word = strtok_r(buffer, " \t\n\r", &saveptr);
    while (word != NULL && *word_count < MAX_WORDS) {
        if (strlen(word) > 0) {
            int found = 0;
            for (int i = 0; i < *word_count; i++) {
                if (strcmp((*words)[i].word, word) == 0) {
                    (*words)[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                char *new_word = safe_strdup(word);
                if (new_word != NULL) {
                    struct WordInfo *temp = realloc(*words, (*word_count + 1) * sizeof(struct WordInfo));
                    if (temp != NULL) {
                        *words = temp;
                        (*words)[*word_count].word = new_word;
                        (*words)[*word_count].count = 1;
                        (*word_count)++;
                    } else {
                        free(new_word);
                    }
                }
            }
        }
        word = strtok_r(NULL, " \t\n\r", &saveptr);
    }
}

void cleanup_words(struct WordInfo *words, int word_count) {
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    free(words);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo *words = NULL;
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    process_string(input, &words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordInfo), compare_words);
        printf("Word frequency analysis:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}