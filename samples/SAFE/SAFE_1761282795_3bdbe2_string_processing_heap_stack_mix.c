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
    if (input == NULL || words == NULL || word_count == NULL) return;
    
    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';
    
    char *word_list[MAX_WORDS];
    int count = 0;
    
    char *token = strtok(buffer, " \t\n\r");
    while (token != NULL && count < MAX_WORDS) {
        if (strlen(token) > 0) {
            word_list[count] = safe_strdup(token);
            if (word_list[count] != NULL) {
                count++;
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (count == 0) {
        *words = NULL;
        *word_count = 0;
        return;
    }
    
    qsort(word_list, count, sizeof(char *), (int (*)(const void *, const void *))strcmp);
    
    struct WordInfo *result = malloc(count * sizeof(struct WordInfo));
    if (result == NULL) {
        for (int i = 0; i < count; i++) {
            free(word_list[i]);
        }
        *words = NULL;
        *word_count = 0;
        return;
    }
    
    int unique_count = 0;
    for (int i = 0; i < count; i++) {
        if (i == 0 || strcmp(word_list[i], word_list[i-1]) != 0) {
            result[unique_count].word = word_list[i];
            result[unique_count].count = 1;
            unique_count++;
        } else {
            result[unique_count-1].count++;
            free(word_list[i]);
        }
    }
    
    *words = realloc(result, unique_count * sizeof(struct WordInfo));
    *word_count = unique_count;
}

void cleanup_words(struct WordInfo *words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
    free(words);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len-1] == '\n') {
        input[len-1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    struct WordInfo *words = NULL;
    int word_count = 0;
    
    process_string(input, &words, &word_count);
    
    if (words == NULL || word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    cleanup_words(words, word_count);
    return 0;
}