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
    
    char *token = strtok(input, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len >= MAX_WORD_LEN - 1) {
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
        return 1;
    }
    
    for (int i = 0; i < word_count; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcasecmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], MAX_WORD_LEN - 1);
                temp[MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_WORD_LEN - 1);
                words[j + 1][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    printf("Sorted words (case-insensitive):\n");
    for (int i = 0; i < word_count; i++) {
        printf("%s\n", words[i]);
    }
    
    int char_count = 0;
    int letter_count = 0;
    int digit_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char_count += strlen(words[i]);
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (isalpha((unsigned char)words[i][j])) {
                letter_count++;
            } else if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total words: %d\n", word_count);
    printf("Total characters: %d\n", char_count);
    printf("Total letters: %d\n", letter_count);
    printf("Total digits: %d\n", digit_count);
    
    return 0;
}