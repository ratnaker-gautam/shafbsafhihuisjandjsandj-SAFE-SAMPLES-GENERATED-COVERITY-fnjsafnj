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
    return isalpha(c) || c == '\'';
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

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
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
    
    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            int start = 0, end = token_len - 1;
            while (start < (int)token_len && !is_word_char(token[start])) start++;
            while (end >= 0 && !is_word_char(token[end])) end--;
            
            if (start <= end) {
                char clean_word[256];
                int clean_len = end - start + 1;
                if (clean_len > 0 && clean_len < 256) {
                    strncpy(clean_word, token + start, clean_len);
                    clean_word[clean_len] = '\0';
                    
                    int found_idx = find_word(words, word_count, clean_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                    } else {
                        words[word_count].word = malloc(strlen(clean_word) + 1);
                        if (words[word_count].word != NULL) {
                            strcpy(words[word_count].word, clean_word);
                            words[word_count].count = 1;
                            word_count++;
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (top 10):\n");
    for (int i = 0; i < word_count && i < 10; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words[i].word);
    }
    free(lower_input);
    
    return 0;
}