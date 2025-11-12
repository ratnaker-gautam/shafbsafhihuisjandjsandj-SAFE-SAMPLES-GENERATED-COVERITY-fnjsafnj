//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    
    printf("Enter text: ");
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
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        size_t word_len = 0;
        
        while (*ptr != '\0' && !isspace((unsigned char)*ptr) && word_len < MAX_WORD_LEN - 1) {
            ptr++;
            word_len++;
        }
        
        if (word_len >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("Found %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nProcessing words...\n");
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > 0) {
            for (size_t j = 0; j < len; j++) {
                words[i][j] = toupper((unsigned char)words[i][j]);
            }
        }
    }
    
    printf("Uppercase words:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    printf("\nReversed order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%d: \"%s\"\n", i + 1, words[i]);
    }
    
    return 0;
}