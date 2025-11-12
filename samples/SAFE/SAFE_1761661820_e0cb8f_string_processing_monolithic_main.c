//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 256
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
        printf("Empty input\n");
        return 0;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            token = strtok(NULL, " \t\n\r");
            continue;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN - 1);
        words[word_count][MAX_WORD_LEN - 1] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("Processing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int has_alpha = 0;
        int has_digit = 0;
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                has_alpha = 1;
            } else if (isdigit((unsigned char)words[i][j])) {
                has_digit = 1;
            }
        }
        
        printf("Word %d: \"%s\" (length: %zu", i + 1, words[i], len);
        
        if (has_alpha && has_digit) {
            printf(", alphanumeric");
        } else if (has_alpha) {
            printf(", alphabetic");
        } else if (has_digit) {
            printf(", numeric");
        } else {
            printf(", other");
        }
        
        printf(")\n");
    }
    
    printf("\nWord statistics:\n");
    printf("Total words: %d\n", word_count);
    
    int longest_index = 0;
    int shortest_index = 0;
    for (int i = 1; i < word_count; i++) {
        if (strlen(words[i]) > strlen(words[longest_index])) {
            longest_index = i;
        }
        if (strlen(words[i]) < strlen(words[shortest_index])) {
            shortest_index = i;
        }
    }
    
    printf("Longest word: \"%s\" (%zu characters)\n", 
           words[longest_index], strlen(words[longest_index]));
    printf("Shortest word: \"%s\" (%zu characters)\n", 
           words[shortest_index], strlen(words[shortest_index]));
    
    return 0;
}