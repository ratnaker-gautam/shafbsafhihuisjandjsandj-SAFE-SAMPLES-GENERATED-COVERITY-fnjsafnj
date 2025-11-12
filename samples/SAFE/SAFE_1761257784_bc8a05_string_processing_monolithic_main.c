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
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN - 1) {
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
    
    printf("\nWord analysis results:\n");
    printf("Total words: %d\n", word_count);
    
    int char_count = 0;
    int digit_count = 0;
    int alpha_count = 0;
    int upper_count = 0;
    int lower_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        char_count += strlen(words[i]);
        for (size_t j = 0; j < strlen(words[i]); j++) {
            if (isdigit((unsigned char)words[i][j])) {
                digit_count++;
            }
            if (isalpha((unsigned char)words[i][j])) {
                alpha_count++;
                if (isupper((unsigned char)words[i][j])) {
                    upper_count++;
                } else {
                    lower_count++;
                }
            }
        }
    }
    
    printf("Total characters in words: %d\n", char_count);
    printf("Digit characters: %d\n", digit_count);
    printf("Alphabetic characters: %d\n", alpha_count);
    printf("Uppercase letters: %d\n", upper_count);
    printf("Lowercase letters: %d\n", lower_count);
    
    printf("\nWord frequency analysis:\n");
    int processed[MAX_WORDS] = {0};
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) continue;
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        printf("'%s': %d occurrence(s)\n", words[i], count);
        processed[i] = 1;
    }
    
    printf("\nLongest word(s): ");
    size_t max_len = 0;
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > max_len) {
            max_len = len;
        }
    }
    
    int first_longest = 1;
    for (int i = 0; i < word_count; i++) {
        if (strlen(words[i]) == max_len) {
            if (!first_longest) {
                printf(", ");
            }
            printf("'%s'", words[i]);
            first_longest = 0;
        }
    }
    printf(" (%zu characters)\n", max_len);
    
    return 0;
}