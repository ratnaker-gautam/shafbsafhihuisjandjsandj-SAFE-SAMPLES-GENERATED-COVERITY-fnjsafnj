//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
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

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
}

int find_word(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return wb->count - wa->count;
}

void process_string(const char *input) {
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return;
    }

    char *lower = to_lowercase(input);
    if (lower == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    WordCount words[MAX_WORDS];
    int word_count = 0;
    char *token = lower;
    
    while (*token && word_count < MAX_WORDS) {
        while (*token && !is_word_char(*token)) token++;
        if (!*token) break;
        
        char *start = token;
        while (*token && is_word_char(*token)) token++;
        
        size_t word_len = token - start;
        if (word_len > 0) {
            char word[256];
            if (word_len >= sizeof(word)) {
                word_len = sizeof(word) - 1;
            }
            strncpy(word, start, word_len);
            word[word_len] = '\0';
            
            int found_idx = find_word(words, word_count, word);
            if (found_idx >= 0) {
                words[found_idx].count++;
            } else {
                char *new_word = malloc(word_len + 1);
                if (new_word == NULL) {
                    printf("Memory allocation failed\n");
                    free(lower);
                    for (int i = 0; i < word_count; i++) free(words[i].word);
                    return;
                }
                strcpy(new_word, word);
                words[word_count].word = new_word;
                words[word_count].count = 1;
                word_count++;
            }
        }
    }

    qsort(words, word_count, sizeof(WordCount), compare_word_count);

    printf("Word frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
        free(words[i].word);
    }

    free(lower);
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text for word frequency analysis: ");
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