//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
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
    return len;
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
    
    printf("Enter text: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = safe_strlen(input, sizeof(input));
    if (input_len == 0 || input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Invalid input length\n");
        return 1;
    }
    
    int i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && IS_WHITESPACE(input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        int word_start = i;
        while (i < input_len && !IS_WHITESPACE(input[i])) {
            i++;
        }
        
        int word_len = i - word_start;
        if (word_len >= MAX_WORD_LEN - 1) {
            word_len = MAX_WORD_LEN - 1;
        }
        
        char current_word[MAX_WORD_LEN];
        int j;
        for (j = 0; j < word_len; j++) {
            current_word[j] = TO_LOWER(input[word_start + j]);
        }
        current_word[j] = '\0';
        
        if (word_len > 0) {
            int found = 0;
            for (int k = 0; k < word_count; k++) {
                if (strcmp(words[k].word, current_word) == 0) {
                    words[k].count++;
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
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}