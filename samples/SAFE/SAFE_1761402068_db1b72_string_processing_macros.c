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
    size_t i;
    for (i = 0; i < n - 1 && src[i] != '\0'; i++) {
        dest[i] = src[i];
    }
    dest[i] = '\0';
    return i;
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
        
        if (i >= input_len) break;
        
        char current_word[MAX_WORD_LEN];
        int j = 0;
        
        while (i < input_len && j < MAX_WORD_LEN - 1 && !IS_WHITESPACE(input[i])) {
            if (IS_ALPHA(input[i])) {
                current_word[j] = TO_LOWER(input[i]);
                j++;
            }
            i++;
        }
        
        if (j > 0) {
            current_word[j] = '\0';
            
            int found = 0;
            for (int k = 0; k < word_count; k++) {
                if (strcmp(words[k].word, current_word) == 0) {
                    words[k].count++;
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                safe_strncpy(words[word_count].word, current_word, sizeof(words[word_count].word));
                words[word_count].count = 1;
                word_count++;
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequencies (alphabetical order):\n");
    for (int k = 0; k < word_count; k++) {
        printf("%s: %d\n", words[k].word, words[k].count);
    }
    
    return 0;
}