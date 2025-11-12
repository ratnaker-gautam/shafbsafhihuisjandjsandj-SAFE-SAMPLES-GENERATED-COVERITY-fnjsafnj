//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: macros ; Variation: string_processing
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

int normalize_word(char *word) {
    int len = safe_strlen(word, MAX_WORD_LEN);
    if (len <= 0) return 0;
    
    int i = 0;
    int j = 0;
    char temp[MAX_WORD_LEN];
    
    while (i < len && j < MAX_WORD_LEN - 1) {
        if (IS_ALPHA(word[i])) {
            temp[j] = TO_LOWER(word[i]);
            j++;
        }
        i++;
    }
    temp[j] = '\0';
    
    if (j > 0) {
        safe_strncpy(word, temp, MAX_WORD_LEN);
        return 1;
    }
    return 0;
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
    
    int input_len = safe_strlen(input, sizeof(input));
    if (input_len <= 0) {
        printf("No input provided\n");
        return 0;
    }
    
    int pos = 0;
    while (pos < input_len && word_count < MAX_WORDS) {
        while (pos < input_len && IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        if (pos >= input_len) break;
        
        int word_start = pos;
        while (pos < input_len && !IS_WHITESPACE(input[pos])) {
            pos++;
        }
        
        int word_len = pos - word_start;
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            int copied = safe_strncpy(temp_word, &input[word_start], word_len + 1);
            temp_word[copied] = '\0';
            
            if (normalize_word(temp_word)) {
                int found = 0;
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(words[i].word, temp_word) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                if (!found) {
                    safe_strncpy(words[word_count].word, temp_word, MAX_WORD_LEN);
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
    }
    
    qsort(words, word_count, sizeof(WordCount), compare_words);
    
    printf("\nWord frequency:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}