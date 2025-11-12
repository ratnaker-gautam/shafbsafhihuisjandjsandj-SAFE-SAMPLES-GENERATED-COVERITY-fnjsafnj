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

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

int find_word(WordCount *words, int count, const char *word) {
    if (words == NULL || word == NULL) return -1;
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i].word, word) == 0) return i;
    }
    return -1;
}

void process_word(char *word) {
    if (word == NULL) return;
    size_t len = strlen(word);
    if (len == 0) return;
    
    for (size_t i = 0; i < len; i++) {
        word[i] = TO_LOWER(word[i]);
    }
    
    while (len > 0 && !isalnum((unsigned char)word[len - 1])) {
        word[--len] = '\0';
    }
    
    size_t start = 0;
    while (start < len && !isalnum((unsigned char)word[start])) {
        start++;
    }
    
    if (start > 0) {
        memmove(word, word + start, len - start + 1);
    }
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
    
    input[strcspn(input, "\n")] = '\0';
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char buffer[MAX_INPUT_LEN];
    if (safe_strncpy(buffer, input, sizeof(buffer)) != 0) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(buffer, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        char processed[MAX_WORD_LEN];
        if (safe_strncpy(processed, token, sizeof(processed)) != 0) {
            token = strtok(NULL, " \t\n");
            continue;
        }
        
        process_word(processed);
        
        if (strlen(processed) > 0) {
            int index = find_word(words, word_count, processed);
            if (index >= 0) {
                if (words[index].count < INT_MAX) {
                    words[index].count++;
                }
            } else {
                if (safe_strncpy(words[word_count].word, processed, sizeof(words[word_count].word)) == 0) {
                    words[word_count].count = 1;
                    word_count++;
                }
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    printf("\nWord frequencies:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s: %d\n", words[i].word, words[i].count);
    }
    
    return 0;
}