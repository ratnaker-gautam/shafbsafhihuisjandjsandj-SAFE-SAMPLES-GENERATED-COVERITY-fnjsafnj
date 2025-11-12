//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) ((c) >= 'A' && (c) <= 'Z' ? (c) + 32 : (c))

typedef struct {
    char word[MAX_WORD_LEN];
    size_t count;
} WordCount;

static size_t parse_words(const char *input, WordCount words[]) {
    size_t word_count = 0;
    const char *p = input;
    
    while (*p != '\0' && word_count < MAX_WORDS) {
        while (IS_WHITESPACE(*p)) p++;
        if (*p == '\0') break;
        
        const char *start = p;
        while (*p != '\0' && !IS_WHITESPACE(*p)) p++;
        
        size_t len = (size_t)(p - start);
        if (len >= MAX_WORD_LEN) len = MAX_WORD_LEN - 1;
        
        char temp[MAX_WORD_LEN];
        for (size_t i = 0; i < len; i++) {
            temp[i] = TO_LOWER(start[i]);
        }
        temp[len] = '\0';
        
        int found = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i].word, temp) == 0) {
                words[i].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            strncpy(words[word_count].word, temp, MAX_WORD_LEN - 1);
            words[word_count].word[MAX_WORD_LEN - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    return word_count;
}

static int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    WordCount words[MAX_WORDS];
    size_t word_count = parse_words(input, words);
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return EXIT_SUCCESS;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_word_count);
    
    printf("\nWord frequency (sorted by count, then alphabetically):\n");
    for (size_t i = 0; i < word_count; i++) {
        printf("%s: %zu\n", words[i].word, words[i].count);
    }
    
    return EXIT_SUCCESS;
}