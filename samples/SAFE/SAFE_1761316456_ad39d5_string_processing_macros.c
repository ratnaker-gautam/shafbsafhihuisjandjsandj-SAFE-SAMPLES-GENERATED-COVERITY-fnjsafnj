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
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        size_t word_start = i;
        size_t word_len = 0;
        char current_word[MAX_WORD_LEN];
        
        while (i < input_len && !IS_WHITESPACE(input[i]) && word_len < MAX_WORD_LEN - 1) {
            char c = input[i];
            if (IS_ALPHA(c)) {
                current_word[word_len] = TO_LOWER(c);
                word_len++;
            }
            i++;
        }
        
        if (word_len > 0) {
            current_word[word_len] = '\0';
            
            int found = 0;
            for (size_t j = 0; j < word_count; j++) {
                if (strcmp(words[j].word, current_word) == 0) {
                    words[j].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count].word, current_word, sizeof(words[word_count].word) - 1);
                    words[word_count].word[sizeof(words[word_count].word) - 1] = '\0';
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequencies (alphabetical order):\n");
    for (size_t j = 0; j < word_count; j++) {
        printf("%s: %zu\n", words[j].word, words[j].count);
    }
    
    return 0;
}