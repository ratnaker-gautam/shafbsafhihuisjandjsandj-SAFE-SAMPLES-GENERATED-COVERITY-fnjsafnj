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

void process_text(const char *text, WordCount *words, int *word_count) {
    char buffer[MAX_WORD_LEN];
    int buf_idx = 0;
    int text_len = strlen(text);
    
    for (int i = 0; i <= text_len; i++) {
        char c = text[i];
        
        if (IS_WHITESPACE(c) || c == '\0') {
            if (buf_idx > 0) {
                buffer[buf_idx] = '\0';
                
                for (int j = 0; j < buf_idx; j++) {
                    buffer[j] = TO_LOWER(buffer[j]);
                }
                
                int existing_idx = find_word_index(words, *word_count, buffer);
                if (existing_idx >= 0) {
                    if (words[existing_idx].count < INT_MAX) {
                        words[existing_idx].count++;
                    }
                } else if (*word_count < MAX_WORDS) {
                    if (safe_strncpy(words[*word_count].word, buffer, MAX_WORD_LEN) == 0) {
                        words[*word_count].count = 1;
                        (*word_count)++;
                    }
                }
                buf_idx = 0;
            }
        } else if (isalpha((unsigned char)c) && buf_idx < MAX_WORD_LEN - 1) {
            buffer[buf_idx++] = c;
        }
    }
}

int compare_word_counts(const void *a, const void *b) {
    const WordCount *wa = (const WordCount *)a;
    const WordCount *wb = (const WordCount *)b;
    if (wa->count > wb->count) return -1;
    if (wa->count < wb->count) return 1;
    return strcmp(wa->word, wb->word);
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text to analyze (max %d characters):\n", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
    }
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    for (int i = 0; i < MAX_WORDS; i++) {
        words[i].word[0] = '\0';
        words[i].count = 0;
    }
    
    process_text(input, words, &word_count);
    
    if (word_count > 0) {
        qsort(words, word_count, sizeof(WordCount), compare_word_counts);
        
        printf("\nWord frequency analysis:\n");
        printf("%-20s %s\n", "Word", "Count");
        printf("%-20s %s\n", "----", "-----");
        
        for (int i = 0; i < word_count; i++) {
            printf("%-20s %d\n", words[i].word, words[i].count);
        }
    } else {
        printf("No words found in input.\n");
    }
    
    return 0;
}