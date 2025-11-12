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
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')

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

int safe_strcmp(const char *s1, const char *s2) {
    if (s1 == NULL || s2 == NULL) return -2;
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
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
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= sizeof(input) - 1 && input[sizeof(input) - 2] != '\n') {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char buffer[MAX_WORD_LEN];
    int buf_pos = 0;
    int in_word = 0;
    
    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];
        
        if (IS_WHITESPACE(c) || IS_TERMINATOR(c) || c == '\0') {
            if (in_word && buf_pos > 0) {
                buffer[buf_pos] = '\0';
                
                int found = -1;
                for (int j = 0; j < word_count; j++) {
                    if (safe_strcmp(words[j].word, buffer) == 0) {
                        found = j;
                        break;
                    }
                }
                
                if (found >= 0) {
                    if (words[found].count < INT_MAX) {
                        words[found].count++;
                    }
                } else if (word_count < MAX_WORDS) {
                    if (safe_strncpy(words[word_count].word, buffer, MAX_WORD_LEN) == 0) {
                        words[word_count].count = 1;
                        word_count++;
                    }
                }
                
                buf_pos = 0;
                in_word = 0;
            }
        } else if (buf_pos < MAX_WORD_LEN - 1) {
            buffer[buf_pos++] = tolower((unsigned char)c);
            in_word = 1;
        }
    }
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}