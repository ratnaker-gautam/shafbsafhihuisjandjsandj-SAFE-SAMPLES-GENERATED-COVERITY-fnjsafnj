//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512
#define MAX_WORD_LEN 64

int main() {
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *pos = input;
    while (*pos && word_count < MAX_WORDS) {
        while (*pos && isspace((unsigned char)*pos)) {
            pos++;
        }
        
        if (!*pos) {
            break;
        }
        
        char *word_start = pos;
        size_t word_len = 0;
        
        while (*pos && !isspace((unsigned char)*pos) && word_len < MAX_WORD_LEN - 1) {
            pos++;
            word_len++;
        }
        
        if (word_len >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        memcpy(words[word_count], word_start, word_len);
        words[word_count][word_len] = '\0';
        word_count++;
        
        if (*pos) {
            pos++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    int total_chars = 0;
    int longest_word_len = 0;
    int shortest_word_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_word_len) {
            longest_word_len = len;
        }
        if (len < shortest_word_len) {
            shortest_word_len = len;
        }
        
        for (size_t j = 0; j < len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Statistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", total_chars);
    printf("Longest word length: %d\n", longest_word_len);
    printf("Shortest word length: %d\n", shortest_word_len);
    printf("Average word length: %.2f\n", word_count > 0 ? (float)total_chars / word_count : 0.0f);
    
    printf("\nWords in uppercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    printf("\nReversed order:\n");
    for (int i = word_count - 1; i >= 0; i--) {
        printf("%s", words[i]);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    
    return 0;
}