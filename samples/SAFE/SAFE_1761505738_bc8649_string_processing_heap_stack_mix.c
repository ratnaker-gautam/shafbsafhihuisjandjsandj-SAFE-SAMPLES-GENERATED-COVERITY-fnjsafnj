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
    size_t len = strlen(s) + 1;
    char *copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, s, len);
    }
    return copy;
}

int is_word_char(char c) {
    return isalnum((unsigned char)c) || c == '_';
}

char *extract_next_word(char **str_ptr) {
    char *str = *str_ptr;
    if (str == NULL) return NULL;
    
    while (*str && !is_word_char(*str)) {
        str++;
    }
    
    if (*str == '\0') {
        *str_ptr = str;
        return NULL;
    }
    
    char *start = str;
    while (*str && is_word_char(*str)) {
        str++;
    }
    
    size_t word_len = str - start;
    char *word = malloc(word_len + 1);
    if (word == NULL) return NULL;
    
    memcpy(word, start, word_len);
    word[word_len] = '\0';
    
    *str_ptr = str;
    return word;
}

int find_word_index(struct WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lower_case(char *str) {
    for (char *p = str; *p; p++) {
        *p = tolower((unsigned char)*p);
    }
}

int compare_word_count(const void *a, const void *b) {
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided.\n");
        return 0;
    }
    
    struct WordCount words_stack[MAX_WORDS];
    struct WordCount *words_heap = NULL;
    int word_count = 0;
    
    char *input_ptr = input;
    char *word;
    
    while ((word = extract_next_word(&input_ptr)) != NULL && word_count < MAX_WORDS) {
        to_lower_case(word);
        
        int existing_index = find_word_index(words_stack, word_count, word);
        
        if (existing_index >= 0) {
            words_stack[existing_index].count++;
            free(word);
        } else {
            if (word_count < MAX_WORDS) {
                words_stack[word_count].word = word;
                words_stack[word_count].count = 1;
                word_count++;
            } else {
                free(word);
            }
        }
    }
    
    if (word_count > 0) {
        words_heap = malloc(sizeof(struct WordCount) * word_count);
        if (words_heap == NULL) {
            for (int i = 0; i < word_count; i++) {
                free(words_stack[i].word);
            }
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < word_count; i++) {
            words_heap[i].word = strdup_safe(words_stack[i].word);
            words_heap[i].count = words_stack[i].count;
            if (words_heap[i].word == NULL) {
                for (int j = 0; j < i; j++) {
                    free(words_heap[j].word);
                }
                free(words_heap);
                for (int j = 0; j < word_count; j++) {
                    free(words_stack[j].word);
                }
                fprintf(stderr, "Memory allocation failed\n");
                return 1;
            }
        }
        
        qsort(words_heap, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("\nWord frequency (sorted by count):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words_heap[i].word, words_heap[i].count);
        }
        
        for (int i = 0; i < word_count; i++) {
            free(words_heap[i].word);
        }
        free(words_heap);
    }
    
    for (int i = 0; i < word_count; i++) {
        free(words_stack[i].word);
    }
    
    return 0;
}