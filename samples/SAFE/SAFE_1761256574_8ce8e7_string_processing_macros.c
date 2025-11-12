//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (dest == NULL || src == NULL || n == 0) return -1;
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return 0;
}

int safe_tolower(int c) {
    return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

int find_word_index(WordCount words[], int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

int process_input(const char *input, WordCount words[], int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_WORD_LEN];
    int buf_index = 0;
    int in_word = 0;
    
    for (size_t i = 0; input[i] != '\0' && i < MAX_INPUT_LEN; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || IS_TERMINATOR(c)) {
            if (in_word && buf_index > 0) {
                buffer[buf_index] = '\0';
                
                if (buf_index >= MAX_WORD_LEN) {
                    buf_index = 0;
                    in_word = 0;
                    continue;
                }
                
                for (int j = 0; buffer[j] != '\0'; j++) {
                    buffer[j] = safe_tolower(buffer[j]);
                }
                
                int index = find_word_index(words, *word_count, buffer);
                if (index >= 0) {
                    if (words[index].count < INT_MAX) {
                        words[index].count++;
                    }
                } else if (*word_count < MAX_WORDS) {
                    if (safe_strncpy(words[*word_count].word, buffer, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                
                buf_index = 0;
                in_word = 0;
            }
        } else if (isalpha((unsigned char)c)) {
            if (buf_index < MAX_WORD_LEN - 1) {
                buffer[buf_index++] = c;
                in_word = 1;
            }
        }
    }
    
    return 0;
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wb->count > wa->count) return 1;
    if (wb->count < wa->count) return -1;
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            if (words[i].count > 0) {
                printf("%-20s %d\n", words[i].word, words[i].count);
            }
        }
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}