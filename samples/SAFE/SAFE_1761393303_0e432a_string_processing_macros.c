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

int find_word_index(WordCount *words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) {
            return i;
        }
    }
    return -1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

int process_input(const char *input, WordCount *words, int *word_count) {
    if (input == NULL || words == NULL || word_count == NULL) return -1;
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) return -1;
    
    to_lowercase(buffer);
    
    int count = 0;
    size_t i = 0;
    
    while (buffer[i] != '\0' && count < MAX_WORDS) {
        while (IS_WHITESPACE(buffer[i]) && buffer[i] != '\0') i++;
        
        if (buffer[i] == '\0') break;
        
        size_t start = i;
        while (!IS_WHITESPACE(buffer[i]) && !IS_TERMINATOR(buffer[i]) && buffer[i] != '\0') i++;
        
        size_t len = i - start;
        if (len > 0 && len < MAX_WORD_LEN) {
            char temp_word[MAX_WORD_LEN];
            if (safe_strncpy(temp_word, &buffer[start], len + 1) != 0) continue;
            
            int index = find_word_index(words, count, temp_word);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (safe_strncpy(words[count].word, temp_word, sizeof(words[count].word)) == 0) {
                    words[count].count = 1;
                    count++;
                }
            }
        }
        
        if (IS_TERMINATOR(buffer[i])) i++;
    }
    
    *word_count = count;
    return 0;
}

void print_word_counts(WordCount *words, int count) {
    if (words == NULL || count <= 0) return;
    
    printf("Word frequencies:\n");
    for (int i = 0; i < count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
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
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (process_input(input, words, &word_count) != 0) {
        fprintf(stderr, "Error processing input\n");
        return 1;
    }
    
    if (word_count > 0) {
        print_word_counts(words, word_count);
    } else {
        printf("No words found in input\n");
    }
    
    return 0;
}