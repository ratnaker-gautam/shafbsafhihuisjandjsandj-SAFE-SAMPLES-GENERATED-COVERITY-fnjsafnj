//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text to process: ");
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
    
    if (input_len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *pos = input;
    while (*pos != '\0' && word_count < MAX_WORDS) {
        while (*pos != '\0' && isspace((unsigned char)*pos)) {
            pos++;
        }
        
        if (*pos == '\0') {
            break;
        }
        
        char *word_start = pos;
        size_t word_len = 0;
        
        while (*pos != '\0' && !isspace((unsigned char)*pos) && word_len < MAX_WORD_LEN - 1) {
            pos++;
            word_len++;
        }
        
        if (word_len >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            memcpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        size_t len = strlen(words[i]);
        if (len > 0) {
            printf("%s", words[i]);
            if (i > 0) {
                printf(" ");
            }
        }
    }
    printf("\n");
    
    printf("Word lengths:\n");
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        printf("'%s': %zu characters\n", words[i], len);
    }
    
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    printf("Total characters in words: %d\n", total_chars);
    
    return 0;
}