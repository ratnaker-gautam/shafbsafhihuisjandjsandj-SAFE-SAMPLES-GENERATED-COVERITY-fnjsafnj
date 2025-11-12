//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 100
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORD_COUNT][MAX_WORD_LEN + 1];
    int word_count = 0;
    int word_lengths[MAX_WORD_COUNT];
    
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
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORD_COUNT) {
        size_t token_len = strlen(token);
        if (token_len > MAX_WORD_LEN) {
            fprintf(stderr, "Word too long: %s\n", token);
            return 1;
        }
        
        strncpy(words[word_count], token, MAX_WORD_LEN);
        words[word_count][MAX_WORD_LEN] = '\0';
        
        int clean_len = 0;
        for (size_t i = 0; i < token_len; i++) {
            if (isalnum((unsigned char)token[i])) {
                clean_len++;
            }
        }
        word_lengths[word_count] = clean_len;
        
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nWord Analysis:\n");
    printf("Total words: %d\n", word_count);
    printf("\nWord lengths (alphanumeric characters only):\n");
    
    int total_length = 0;
    int min_length = MAX_WORD_LEN;
    int max_length = 0;
    
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: \"%s\" - Length: %d\n", i + 1, words[i], word_lengths[i]);
        total_length += word_lengths[i];
        
        if (word_lengths[i] < min_length) {
            min_length = word_lengths[i];
        }
        if (word_lengths[i] > max_length) {
            max_length = word_lengths[i];
        }
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_length / word_count;
        printf("\nStatistics:\n");
        printf("Average word length: %.2f\n", avg_length);
        printf("Shortest word length: %d\n", min_length);
        printf("Longest word length: %d\n", max_length);
    }
    
    printf("\nProcessing complete.\n");
    return 0;
}