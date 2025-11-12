//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    
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
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        word_count++;
        
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 0;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    
    for (int i = 0; i < word_count; i++) {
        char processed[MAX_WORD_LEN + 1];
        size_t len = strlen(words[i]);
        size_t processed_idx = 0;
        
        for (size_t j = 0; j < len; j++) {
            if (isalpha((unsigned char)words[i][j])) {
                processed[processed_idx++] = tolower((unsigned char)words[i][j]);
            }
        }
        processed[processed_idx] = '\0';
        
        if (processed_idx > 0) {
            printf("Original: %-15s Processed: %s\n", words[i], processed);
        } else {
            printf("Original: %-15s Processed: [no letters]\n", words[i]);
        }
    }
    
    printf("\nWord statistics:\n");
    printf("Total words processed: %d\n", word_count);
    
    int letter_count = 0;
    for (int i = 0; i < word_count; i++) {
        letter_count += strlen(words[i]);
    }
    printf("Total letters: %d\n", letter_count);
    
    int longest_len = 0;
    for (int i = 0; i < word_count; i++) {
        int current_len = strlen(words[i]);
        if (current_len > longest_len) {
            longest_len = current_len;
        }
    }
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}