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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

char *to_lowercase(const char *str) {
    if (str == NULL) return NULL;
    size_t len = strlen(str);
    if (len == 0) return NULL;
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

int process_words(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return 0;
    
    char *lower = to_lowercase(input);
    if (lower == NULL) return 0;
    
    int count = 0;
    size_t len = strlen(lower);
    size_t i = 0;
    
    while (i < len && count < MAX_WORDS) {
        while (i < len && !is_word_char(lower[i])) i++;
        if (i >= len) break;
        
        size_t start = i;
        while (i < len && is_word_char(lower[i])) i++;
        size_t word_len = i - start;
        
        if (word_len > 0 && word_len <= MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN + 1];
            if (word_len >= sizeof(temp_word)) {
                free(lower);
                return 0;
            }
            strncpy(temp_word, lower + start, word_len);
            temp_word[word_len] = '\0';
            
            int found = 0;
            for (int j = 0; j < count; j++) {
                if (strcmp(words[j].word, temp_word) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                words[count].word = malloc(word_len + 1);
                if (words[count].word == NULL) {
                    free(lower);
                    return 0;
                }
                strcpy(words[count].word, temp_word);
                words[count].count = 1;
                count++;
            }
        }
    }
    
    *word_count = count;
    free(lower);
    return 1;
}

void cleanup_words(WordCount *words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i].word);
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    WordCount words_stack[MAX_WORDS];
    
    printf("Enter text for word frequency analysis: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    int word_count = 0;
    if (!process_words(input, words_stack, &word_count)) {
        printf("Error processing words\n");
        return 1;
    }
    
    if (word_count > 0) {
        WordCount *words_heap = malloc(word_count * sizeof(WordCount));
        if (words_heap == NULL) {
            cleanup_words(words_stack, word_count);
            printf("Memory allocation failed\n");
            return 1;
        }
        
        for (int i = 0; i < word_count; i++) {
            size_t len = strlen(words_stack[i].word) + 1;
            words_heap[i].word = malloc(len);
            if (words_heap[i].word == NULL) {
                for (int j = 0; j < i; j++) {
                    free(words_heap[j].word);
                }
                free(words_heap);
                cleanup_words(words_stack, word_count);
                printf("Memory allocation failed\n");
                return 1;
            }
            strcpy(words_heap[i].word, words_stack[i].word);
            words_heap[i].count = words_stack[i].count;
        }
        
        qsort(words_heap, word_count, sizeof(WordCount