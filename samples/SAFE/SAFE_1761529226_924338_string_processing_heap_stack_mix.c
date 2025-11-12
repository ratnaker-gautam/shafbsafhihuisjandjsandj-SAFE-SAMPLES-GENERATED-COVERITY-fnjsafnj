//DeepSeek-V3 V2.5 Category: Safe ; Style: heap_stack_mix ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

typedef struct {
    char *word;
    int count;
} WordCount;

int is_valid_char(char c) {
    return isalnum((unsigned char)c) || c == '-' || c == '\'';
}

char *strdup_safe(const char *s) {
    if (s == NULL) return NULL;
    size_t len = strlen(s);
    if (len == 0) return NULL;
    char *copy = malloc(len + 1);
    if (copy == NULL) return NULL;
    strcpy(copy, s);
    return copy;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

void process_word(char *word, WordCount *words, int *word_count) {
    if (word[0] == '\0') return;
    
    for (int i = 0; i < *word_count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            words[i].count++;
            return;
        }
    }
    
    if (*word_count < MAX_WORDS) {
        words[*word_count].word = strdup_safe(word);
        if (words[*word_count].word != NULL) {
            words[*word_count].count = 1;
            (*word_count)++;
        }
    }
}

void cleanup_words(WordCount *words, int count) {
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words_stack[MAX_WORDS];
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
        return 0;
    }
    
    char *word_heap = malloc(input_len + 1);
    if (word_heap == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    int word_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (is_valid_char(c) && word_pos < MAX_WORD_LEN - 1) {
            word_heap[word_pos++] = tolower((unsigned char)c);
            in_word = 1;
        } else {
            if (in_word) {
                word_heap[word_pos] = '\0';
                process_word(word_heap, words_stack, &word_count);
                word_pos = 0;
                in_word = 0;
            }
        }
    }
    
    free(word_heap);
    
    if (word_count > 0) {
        qsort(words_stack, word_count, sizeof(WordCount), compare_words);
        
        printf("\nWord frequencies:\n");
        for (int i = 0; i < word_count; i++) {
            if (words_stack[i].word != NULL) {
                printf("%s: %d\n", words_stack[i].word, words_stack[i].count);
            }
        }
    } else {
        printf("No valid words found\n");
    }
    
    cleanup_words(words_stack, word_count);
    return 0;
}