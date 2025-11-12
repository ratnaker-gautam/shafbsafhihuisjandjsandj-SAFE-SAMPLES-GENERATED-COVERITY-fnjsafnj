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

void process_words(struct WordInfo *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    qsort(words, word_count, sizeof(struct WordInfo), compare_words);
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i].word != NULL) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    }
}

void cleanup_words(struct WordInfo *words, int word_count) {
    if (words == NULL) return;
    for (int i = 0; i < word_count; i++) {
        if (words[i].word != NULL) {
            free(words[i].word);
        }
    }
    free(words);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordInfo *words = NULL;
    int word_count = 0;
    
    printf("Enter text to analyze: ");
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
    
    words = malloc(MAX_WORDS * sizeof(struct WordInfo));
    if (words == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word = NULL;
        words[i].count = 0;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (words[i].word != NULL && strcmp(words[i].word, token) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                char *word_copy = safe_strdup(token);
                if (word_copy != NULL) {
                    words[word_count].word = word_copy;
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count > 0) {
        process_words(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    cleanup_words(words, word_count);
    return 0;
}