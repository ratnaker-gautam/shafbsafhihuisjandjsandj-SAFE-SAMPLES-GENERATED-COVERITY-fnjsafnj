//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
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
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < 64) {
            strncpy(words[word_count], token, 63);
            words[word_count][63] = '\0';
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
    
    printf("\nWord frequency analysis:\n");
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (words[j][0] != '\0' && strcmp(words[i], words[j]) == 0) {
                count++;
                words[j][0] = '\0';
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
    }
    
    printf("\nWord lengths:\n");
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] != '\0') {
            size_t len = strlen(words[i]);
            total_chars += len;
            printf("'%s': %zu character(s)\n", words[i], len);
        }
    }
    
    if (word_count > 0) {
        double avg_length = (double)total_chars / word_count;
        printf("\nAverage word length: %.2f\n", avg_length);
    }
    
    printf("\nCase analysis:\n");
    int upper_count = 0, lower_count = 0, mixed_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (words[i][0] == '\0') continue;
        
        int has_upper = 0, has_lower = 0;
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (isupper((unsigned char)words[i][j])) {
                has_upper = 1;
            } else if (islower((unsigned char)words[i][j])) {
                has_lower = 1;
            }
        }
        
        if (has_upper && has_lower) {
            mixed_count++;
        } else if (has_upper) {
            upper_count++;
        } else if (has_lower) {
            lower_count++;
        }
    }
    
    printf("Uppercase words: %d\n", upper_count);
    printf("Lowercase words: %d\n", lower_count);
    printf("Mixed case words: %d\n", mixed_count);
    
    return 0;
}