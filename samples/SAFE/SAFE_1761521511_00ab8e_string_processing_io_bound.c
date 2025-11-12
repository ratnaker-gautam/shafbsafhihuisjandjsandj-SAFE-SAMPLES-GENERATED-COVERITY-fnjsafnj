//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][MAX_WORD_LEN];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN - 1);
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
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        int has_upper = 0;
        int has_lower = 0;
        int has_digit = 0;
        
        for (size_t j = 0; j < len; j++) {
            if (isupper((unsigned char)words[i][j])) has_upper = 1;
            if (islower((unsigned char)words[i][j])) has_lower = 1;
            if (isdigit((unsigned char)words[i][j])) has_digit = 1;
        }
        
        printf("Word %d: \"%s\" (length: %zu)", i + 1, words[i], len);
        
        if (has_upper && has_lower) {
            printf(" - Mixed case");
        } else if (has_upper) {
            printf(" - All uppercase");
        } else if (has_lower) {
            printf(" - All lowercase");
        }
        
        if (has_digit) {
            printf(" - Contains digits");
        }
        
        printf("\n");
    }
    
    printf("\nWord statistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int total_chars = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        total_chars += (int)len;
        if ((int)len < shortest_len) shortest_len = (int)len;
        if ((int)len > longest_len) longest_len = (int)len;
    }
    
    printf("Average word length: %.2f\n", word_count > 0 ? (double)total_chars / word_count : 0.0);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}