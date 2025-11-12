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
    if (dup != NULL) {
        strcpy(dup, s);
    }
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
    
    char *token = strtok(lower_input, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        if (token_len > 0) {
            size_t start = 0;
            size_t end = token_len - 1;
            
            while (start < token_len && !is_word_char(token[start])) start++;
            while (end > start && !is_word_char(token[end])) end--;
            
            if (start <= end) {
                size_t word_len = end - start + 1;
                char *clean_word = malloc(word_len + 1);
                if (clean_word == NULL) {
                    free(lower_input);
                    free_words(words, word_count);
                    fprintf(stderr, "Memory allocation failed\n");
                    return 1;
                }
                strncpy(clean_word, token + start, word_len);
                clean_word[word_len] = '\0';
                
                if (strlen(clean_word) > 0) {
                    int found_idx = find_word(words, word_count, clean_word);
                    if (found_idx >= 0) {
                        words[found_idx].count++;
                        free(clean_word);
                    } else {
                        words[word_count].word = clean_word;
                        words[word_count].count = 1;
                        word_count++;
                    }
                } else {
                    free(clean_word);
                }
            }
        }
        
        token = strtok(NULL, " ");
    }
    
    free(lower_input);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("\nWord frequency (top %d):\n", word_count > 10 ? 10 : word_count);
        int limit = word_count > 10 ? 10 : word_count;
        for (int i = 0; i < limit; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found\n");
    }
    
    free_words(words, word_count);
    return 0;
}