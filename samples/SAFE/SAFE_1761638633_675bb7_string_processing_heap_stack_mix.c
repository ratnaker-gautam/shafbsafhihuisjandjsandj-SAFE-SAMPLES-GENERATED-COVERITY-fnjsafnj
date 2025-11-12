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

int find_word_index(struct WordCount *words, int count, const char *word) {
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
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    return wb->count - wa->count;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    struct WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
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
                size_t clean_len = end - start + 1;
                if (clean_len >= sizeof(clean_word)) {
                    token = strtok(NULL, " ");
                    continue;
                }
                strncpy(clean_word, token + start, clean_len);
                clean_word[clean_len] = '\0';
                
                if (strlen(clean_word) > 0) {
                    int index = find_word_index(words, word_count, clean_word);
                    if (index >= 0) {
                        words[index].count++;
                    } else {
                        words[word_count].word = malloc(strlen(clean_word) + 1);
                        if (words[word_count].word == NULL) {
                            free(lower_input);
                            free_words(words, word_count);
                            fprintf(stderr, "Memory allocation failed\n");
                            return 1;
                        }
                        strcpy(words[word_count].word, clean_word);
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }
    
    qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    free(lower_input);
    free_words(words, word_count);
    
    return 0;
}