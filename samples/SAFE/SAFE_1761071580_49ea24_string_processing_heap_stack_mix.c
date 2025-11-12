//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
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

void process_string(const char *input) {
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return;
    }

    char buffer[MAX_INPUT_LEN + 1];
    strncpy(buffer, input, MAX_INPUT_LEN);
    buffer[MAX_INPUT_LEN] = '\0';

    struct WordInfo *word_array = malloc(MAX_WORDS * sizeof(struct WordInfo));
    if (word_array == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    int word_count = 0;
    char *token = strtok(buffer, " \t\n\r");
    
    while (token != NULL && word_count < MAX_WORDS) {
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(word_array[i].word, token) == 0) {
                word_array[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            char *word_copy = safe_strdup(token);
            if (word_copy == NULL) {
                printf("Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(word_array[i].word);
                }
                free(word_array);
                return;
            }
            word_array[word_count].word = word_copy;
            word_array[word_count].count = 1;
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }

    qsort(word_array, word_count, sizeof(struct WordInfo), compare_words);

    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", word_array[i].word, word_array[i].count);
    }

    for (int i = 0; i < word_count; i++) {
        free(word_array[i].word);
    }
    free(word_array);
}

int main() {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter a string for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_string(input);
    
    return 0;
}