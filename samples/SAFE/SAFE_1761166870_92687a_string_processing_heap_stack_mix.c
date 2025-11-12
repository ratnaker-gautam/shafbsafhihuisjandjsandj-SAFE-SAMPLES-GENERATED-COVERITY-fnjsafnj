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

int is_valid_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(input[i])) return 0;
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

int compare_words(const void *a, const void *b) {
    const struct WordInfo *wa = (const struct WordInfo *)a;
    const struct WordInfo *wb = (const struct WordInfo *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *lower_input = to_lowercase(input);
    if (lower_input == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    struct WordInfo words[MAX_WORDS];
    int word_count = 0;
    char *token = strtok(lower_input, " \t\n\r.,;:!?");
    
    while (token != NULL && word_count < MAX_WORDS) {
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, token) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            size_t token_len = strlen(token);
            if (token_len > 0) {
                words[word_count].word = malloc(token_len + 1);
                if (words[word_count].word == NULL) {
                    fprintf(stderr, "Memory allocation failed\n");
                    for (int i = 0; i < word_count; i++) {
                        free(words[i].word);
                    }
                    free(lower_input);
                    return 1;
                }
                strcpy(words[word_count].word, token);
                words[word_count].count = 1;
                word_count++;
            }
        }
        token = strtok(NULL, " \t\n\r.,;:!?");
    }
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_words);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    free(lower_input);
    
    return 0;
}