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

char *strtolower(char *str) {
    if (str == NULL) return NULL;
    char *p = str;
    while (*p) {
        *p = tolower(*p);
        p++;
    }
    return str;
}

int is_word_char(char c) {
    return isalnum(c) || c == '\'' || c == '-';
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
    const struct WordCount *wa = (const struct WordCount *)a;
    const struct WordCount *wb = (const struct WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    struct WordCount words_stack[MAX_WORDS];
    struct WordCount *words = words_stack;
    int word_count = 0;
    int use_heap = 0;

    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }

    if (len == 0) {
        printf("No input provided\n");
        return 0;
    }

    char *copy = malloc(len + 1);
    if (copy == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    strcpy(copy, input);
    strtolower(copy);

    char *token = strtok(copy, " ");
    while (token != NULL && word_count < MAX_WORDS) {
        int token_len = strlen(token);
        if (token_len > 0) {
            int start = 0;
            int end = token_len - 1;
            while (start < token_len && !is_word_char(token[start])) start++;
            while (end >= 0 && !is_word_char(token[end])) end--;
            
            if (start <= end) {
                int word_len = end - start + 1;
                char word[word_len + 1];
                strncpy(word, token + start, word_len);
                word[word_len] = '\0';
                
                if (strlen(word) > 0) {
                    int index = find_word(words, word_count, word);
                    if (index >= 0) {
                        words[index].count++;
                    } else {
                        if (word_count >= MAX_WORDS / 2 && !use_heap) {
                            struct WordCount *new_words = malloc(MAX_WORDS * sizeof(struct WordCount));
                            if (new_words != NULL) {
                                memcpy(new_words, words, word_count * sizeof(struct WordCount));
                                words = new_words;
                                use_heap = 1;
                            }
                        }
                        
                        if (word_count < MAX_WORDS) {
                            words[word_count].word = malloc(strlen(word) + 1);
                            if (words[word_count].word != NULL) {
                                strcpy(words[word_count].word, word);
                                words[word_count].count = 1;
                                word_count++;
                            }
                        }
                    }
                }
            }
        }
        token = strtok(NULL, " ");
    }

    free(copy);

    if (word_count > 0) {
        qsort(words, word_count, sizeof(struct WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No valid words found in input\n");
    }

    if (use_heap) {
        free_words(words, word_count);
        free(words);
    } else {
        free_words(words_stack, word_count);
    }

    return 0;
}