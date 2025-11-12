//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char *words[MAX_WORDS];
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
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    
    int total_chars = 0;
    int longest_len = 0;
    int shortest_len = 1000;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += len;
        
        if (len > longest_len) {
            longest_len = len;
        }
        if (len < shortest_len) {
            shortest_len = len;
        }
        
        for (size_t j = 0; j < len; j++) {
            words[i][j] = toupper((unsigned char)words[i][j]);
        }
    }
    
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    printf("Longest word length: %d\n", longest_len);
    printf("Shortest word length: %d\n", shortest_len);
    
    printf("\nWords in uppercase:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s ", words[i]);
    }
    printf("\n");
    
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    
    return 0;
}