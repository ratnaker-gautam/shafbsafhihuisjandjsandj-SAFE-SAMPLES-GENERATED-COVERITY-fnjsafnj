//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char word[MAX_WORD_LEN];
    size_t count;
} WordCount;

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    size_t word_count = 0;
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len >= sizeof(input) - 1 && input[input_len - 1] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < input_len) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        size_t word_len = i - start;
        if (word_len >= MAX_WORD_LEN - 1) {
            word_len = MAX_WORD_LEN - 2;
        }
        
        char current_word[MAX_WORD_LEN];
        size_t valid_chars = 0;
        for (size_t j = 0; j < word_len; j++) {
            char c = input[start + j];
            if (IS_ALPHA(c)) {
                current_word[valid_chars++] = TO_LOWER(c);
            }
        }
        
        if (valid_chars == 0) {
            continue;
        }
        
        current_word[valid_chars] = '\0';
        
        int found = 0;
        for (size_t k = 0; k < word_count; k++) {
            if (strcmp(words[k].word, current_word) == 0) {
                words[k].count++;
                found = 1;
                break;
            }
        }
        
        if (!found) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many unique words\n");
                return 1;
            }
            strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
            words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
            words[word_count].count = 1;
            word_count++;
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("Word frequencies:\n");
    for (size_t i = 0; i < word_count; i++) {
        printf("%s: %zu\n", words[i].word, words[i].count);
    }
    
    return 0;
}