//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORD_COUNT 256
#define MAX_WORD_LEN 64

int main() {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORD_COUNT][MAX_WORD_LEN];
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORD_COUNT) {
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
        return 1;
    }
    
    printf("\nProcessing results:\n");
    printf("Total words: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        char processed[MAX_WORD_LEN];
        size_t word_len = strlen(words[i]);
        
        if (word_len >= MAX_WORD_LEN) {
            continue;
        }
        
        strncpy(processed, words[i], sizeof(processed) - 1);
        processed[sizeof(processed) - 1] = '\0';
        
        for (size_t j = 0; j < word_len; j++) {
            processed[j] = toupper((unsigned char)processed[j]);
        }
        
        printf("%s", processed);
        if (i > 0) {
            printf(" ");
        }
    }
    printf("\n");
    
    printf("\nWord statistics:\n");
    int total_chars = 0;
    for (int i = 0; i < word_count; i++) {
        total_chars += (int)strlen(words[i]);
    }
    printf("Average word length: %.2f\n", (float)total_chars / word_count);
    
    int longest_index = 0;
    for (int i = 1; i < word_count; i++) {
        if (strlen(words[i]) > strlen(words[longest_index])) {
            longest_index = i;
        }
    }
    printf("Longest word: %s (%zu characters)\n", words[longest_index], strlen(words[longest_index]));
    
    return 0;
}