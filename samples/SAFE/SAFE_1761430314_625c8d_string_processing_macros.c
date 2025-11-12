//DeepSeek-V3 V2.5 Category: Safe ; Style: macros ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30
#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n')
#define IS_TERMINATOR(c) ((c) == '.' || (c) == '!' || (c) == '?')
#define SAFE_STRCPY(dest, src, max) do { \
    strncpy((dest), (src), (max) - 1); \
    (dest)[(max) - 1] = '\0'; \
} while(0)

typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN];
    WordCount word_counts[MAX_WORDS];
    int word_count = 0;
    
    printf("Enter a sentence (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    SAFE_STRCPY(temp, input, sizeof(temp));
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        
        if (token_len > 0 && IS_TERMINATOR(token[token_len - 1])) {
            token[token_len - 1] = '\0';
            token_len--;
        }
        
        if (token_len > 0) {
            for (size_t i = 0; i < token_len; i++) {
                token[i] = tolower((unsigned char)token[i]);
            }
            
            if (token_len < MAX_WORD_LEN) {
                SAFE_STRCPY(words[word_count], token, MAX_WORD_LEN);
                word_count++;
            }
        }
        
        token = strtok(NULL, " \t\n");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], word_counts[j].word) == 0) {
                word_counts[j].count++;
                found = 1;
                break;
            }
        }
        
        if (!found && unique_count < MAX_WORDS) {
            SAFE_STRCPY(word_counts[unique_count].word, words[i], MAX_WORD_LEN);
            word_counts[unique_count].count = 1;
            unique_count++;
        }
    }
    
    printf("\nWord frequency analysis:\n");
    printf("Original: %s\n", input);
    printf("Total words: %d\n", word_count);
    printf("Unique words: %d\n\n", unique_count);
    
    for (int i = 0; i < unique_count; i++) {
        printf("%s: %d\n", word_counts[i].word, word_counts[i].count);
    }
    
    return 0;
}