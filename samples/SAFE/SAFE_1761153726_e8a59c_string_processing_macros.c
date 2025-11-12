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

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int safe_strlen(const char *str, size_t max_len) {
    size_t len = 0;
    while (len < max_len && str[len] != '\0') {
        len++;
    }
    return (len < max_len) ? (int)len : -1;
}

int compare_words(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
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
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len < 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int pos = 0;
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) {
            break;
        }
        
        int word_start = pos;
        int word_len = 0;
        char current_word[MAX_WORD_LEN];
        
        while (pos < input_len && !IS_WHITESPACE(input[pos]) && word_len < MAX_WORD_LEN - 1) {
            char c = input[pos];
            if (isalpha((unsigned char)c)) {
                current_word[word_len] = TO_LOWER(c);
                word_len++;
            }
            pos++;
        }
        
        if (word_len > 0) {
            current_word[word_len] = '\0';
            
            int found = 0;
            for (int i = 0; i < word_count; i++) {
                if (strcmp(words[i].word, current_word) == 0) {
                    words[i].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, current_word, MAX_WORD_LEN - 1);
                    words[word_count].word[MAX_WORD_LEN - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    qsort(words, (size_t)word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}