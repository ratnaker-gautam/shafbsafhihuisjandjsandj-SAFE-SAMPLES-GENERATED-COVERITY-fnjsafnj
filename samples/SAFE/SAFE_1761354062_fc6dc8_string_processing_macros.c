//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))
#define IS_ALPHA(c) (((c) >= 'A' && (c) <= 'Z') || ((c) >= 'a' && (c) <= 'z'))

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return len;
}

int safe_strncpy(char *dest, const char *src, size_t n) {
    if (n == 0) return 0;
    size_t i = 0;
    while (i < n - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return i;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    return strcmp(wa->word, wb->word);
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    int count = 0;
    size_t input_len = safe_strlen(input, MAX_INPUT_LEN);
    size_t pos = 0;
    
    while (pos < input_len && count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        char current_word[MAX_WORD_LEN];
        size_t word_pos = 0;
        
        while (pos < input_len && word_pos < MAX_WORD_LEN - 1 && 
               !IS_WHITESPACE(input[pos])) {
            char c = input[pos];
            if (IS_ALPHA(c)) {
                current_word[word_pos++] = TO_LOWER(c);
            }
            pos++;
        }
        
        if (word_pos > 0) {
            current_word[word_pos] = '\0';
            
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (strcmp(words[i].word, current_word) == 0) {
                    if (words[i].count < INT_MAX) {
                        words[i].count++;
                    }
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[count].word, current_word, MAX_WORD_LEN) > 0) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
    }
    
    *word_count = count;
    return 0;
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = safe_strlen(input, MAX_INPUT_LEN);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\0')) {
        printf("No input provided\n");
        return 1;
    }
    
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("Word frequencies (alphabetical order):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}