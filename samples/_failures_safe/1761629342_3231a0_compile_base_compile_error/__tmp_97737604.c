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

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void process_input(const char *input, WordCount *words, int *word_count) {
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return;
    
    char temp_word[MAX_WORD_LEN];
    int buf_index = 0;
    int word_len = 0;
    
    while (buf_index < MAX_INPUT_LEN - 1 && buffer[buf_index] != '\0') {
        if (IS_WHITESPACE(buffer[buf_index])) {
            if (word_len > 0) {
                temp_word[word_len] = '\0';
                
                for (int i = 0; i < word_len; i++) {
                    temp_word[i] = TO_LOWER(temp_word[i]);
                }
                
                int found_index = find_word_index(words, *word_count, temp_word);
                if (found_index >= 0) {
                    if (words[found_index].count < INT_MAX) {
                        words[found_index].count++;
                    }
                } else if (*word_count < MAX_WORDS - 1) {
                    if (safe_strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                word_len = 0;
            }
        } else if (isalpha((unsigned char)buffer[buf_index])) {
            if (word_len < MAX_WORD_LEN - 1) {
                temp_word[word_len++] = buffer[buf_index];
            }
        }
        buf_index++;
    }
    
    if (word_len > 0) {
        temp_word[word_len] = '\0';
        for (int i = 0; i < word_len; i++) {
            temp_word[i] = TO_LOWER(temp_word[i]);
        }
        
        int found_index = find_word_index(words, *word_count, temp_word);
        if (found_index >= 0) {
            if (words[found_index].count < INT_MAX) {
                words[found_index].count++;
            }
        } else if (*word_count < MAX_WORDS - 1) {
            if (safe_strncpy(words[*word_count].word, temp_word, MAX_WORD_LEN) == 0) {
                words[*word_count].count = 1;
                (*word_count)++;
            }
        }
    }
}

int compare_word_count(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count != wb->count) {
        return wb->count - wa->count;
    }
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
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
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    process_input(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_count);
        
        printf("\nWord frequency count (sorted by frequency):\n");
        for (int i = 0; i < word_count; i++) {
            printf("%s: %d\n", words[i].word, words[i].count);
        }
    } else {