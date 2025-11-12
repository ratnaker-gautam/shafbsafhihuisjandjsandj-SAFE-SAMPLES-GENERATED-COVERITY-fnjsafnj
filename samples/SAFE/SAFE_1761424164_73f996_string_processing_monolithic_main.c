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
    
    printf("Enter text to analyze: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len == 0 || (input_len == 1 && input[0] == '\n')) {
        printf("No input provided\n");
        return 0;
    }
    
    if (input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
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
        
        if (word_len > 0 && word_len < MAX_WORD_LEN) {
            memcpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
        
        if (*pos && !isspace((unsigned char)*pos)) {
            fprintf(stderr, "Word too long, skipping remainder\n");
            while (*pos && !isspace((unsigned char)*pos)) {
                pos++;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nWord analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = MAX_WORD_LEN;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += (int)len;
        
        if ((int)len > longest_len) {
            longest_len = (int)len;
        }
        if ((int)len < shortest_len) {
            shortest_len = (int)len;
        }
    }
    
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords sorted by length:\n");
    
    for (int len = shortest_len; len <= longest_len; len++) {
        for (int i = 0; i < word_count; i++) {
            if ((int)strlen(words[i]) == len) {
                printf("%s ", words[i]);
            }
        }
    }
    printf("\n");
    
    return 0;
}