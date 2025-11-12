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
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

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

int is_alpha_char(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    int count = 0;
    size_t input_len = strlen(input);
    if (input_len >= MAX_INPUT_LEN) return -1;
    
    size_t i = 0;
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) i++;
        if (i >= input_len) break;
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && is_alpha_char(input[i])) i++;
        size_t word_len = i - start;
        
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            for (size_t j = 0; j < word_len; j++) {
                temp_word[j] = TO_LOWER(input[start + j]);
            }
            temp_word[word_len] = '\0';
            
            int found = 0;
            for (int k = 0; k < count; k++) {
                if (strcmp(words[k].word, temp_word) == 0) {
                    if (words[k].count < INT_MAX) words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN) == 0) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
    }
    
    *word_count = count;
    return 0;
}

void print_results(const WordCount *words, int word_count) {
    if (words == NULL || word_count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
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
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        print_results(words, word_count);
    } else {
        printf("No valid words found\n");
    }
    
    return 0;
}