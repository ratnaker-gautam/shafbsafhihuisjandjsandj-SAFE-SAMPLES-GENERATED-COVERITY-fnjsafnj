//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
#define MAX_WORD_LEN 64
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')
#define TO_LOWER(c) (((c) >= 'A' && (c) <= 'Z') ? ((c) + 32) : (c))

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

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    int count = 0;
    char *ptr = buffer;
    
    while (*ptr != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(*ptr)) ptr++;
        if (*ptr == '\0') break;
        
        char current_word[MAX_WORD_LEN];
        int word_len = 0;
        
        while (*ptr != '\0' && !IS_WHITESPACE(*ptr) && word_len < MAX_WORD_LEN - 1) {
            current_word[word_len] = TO_LOWER(*ptr);
            word_len++;
            ptr++;
        }
        current_word[word_len] = '\0';
        
        if (word_len > 0) {
            int found = 0;
            for (int i = 0; i < count; i++) {
                if (safe_strcmp(words[i].word, current_word) == 0) {
                    if (words[i].count < INT_MAX) {
                        words[i].count++;
                    }
                    found = 1;
                    break;
                }
            }
            
            if (!found) {
                if (safe_strncpy(words[count].word, current_word, sizeof(words[count].word)) == 0) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
    }
    
    *word_count = count;
    return 0;
}

void sort_words(WordCount *words, int count) {
    if (words == NULL || count <= 1) return;
    
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (safe_strcmp(words[j].word, words[j+1].word) > 0) {
                WordCount temp = words[j];
                words[j] = words[j+1];
                words[j+1] = temp;
            }
        }
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    WordCount words[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return EXIT_SUCCESS;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return EXIT_FAILURE;
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return EXIT_SUCCESS;
    }
    
    sort_words(words, word_count);
    
    printf("\nWord frequencies (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return EXIT_SUCCESS;
}