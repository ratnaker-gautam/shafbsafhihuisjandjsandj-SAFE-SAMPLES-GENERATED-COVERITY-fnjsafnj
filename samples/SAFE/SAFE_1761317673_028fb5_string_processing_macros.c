//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

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

static int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

static void safe_strncpy(char *dest, const char *src, size_t n) {
    if (n > 0) {
        size_t i;
        for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
            dest[i] = src[i];
        }
        dest[i] = '\0';
    }
}

static int process_input(const char *input, WordCount *words, size_t *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) {
        return 0;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || input_len >= MAX_INPUT_LEN) {
        return 0;
    }

    size_t count = 0;
    size_t i = 0;
    
    while (i < input_len && count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t start = i;
        while (i < input_len && !IS_WHITESPACE(input[i]) && (i - start) < MAX_WORD_LEN - 1) {
            i++;
        }
        
        if (i > start) {
            char temp_word[MAX_WORD_LEN];
            size_t j;
            size_t temp_len = 0;
            
            for (j = start; j < i && temp_len < MAX_WORD_LEN - 1; j++) {
                if (IS_ALPHA(input[j])) {
                    temp_word[temp_len++] = TO_LOWER(input[j]);
                }
            }
            temp_word[temp_len] = '\0';
            
            if (temp_len > 0) {
                int found = 0;
                size_t k;
                for (k = 0; k < count; k++) {
                    if (strcmp(words[k].word, temp_word) == 0) {
                        if (words[k].count < SIZE_MAX) {
                            words[k].count++;
                        }
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    safe_strncpy(words[count].word, temp_word, MAX_WORD_LEN);
                    words[count].count = 1;
                    count++;
                }
            }
        }
    }
    
    *word_count = count;
    return 1;
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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (!process_input(input, words, &word_count)) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_words);
        
        printf("Word frequencies (alphabetical order):\n");
        size_t i;
        for (i = 0; i < word_count; i++) {
            printf("%s: %zu\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input.\n");
    }
    
    return 0;
}