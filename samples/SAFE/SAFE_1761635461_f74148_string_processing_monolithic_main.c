//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int char_count = 0;
    int line_count = 0;
    
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
        printf("Empty input provided\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char_count = input_len;
    
    for (int i = 0; i < input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    line_count++;
    
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
    
    printf("\nText Analysis Results:\n");
    printf("Characters: %d\n", char_count);
    printf("Lines: %d\n", line_count);
    printf("Words: %d\n", word_count);
    
    if (word_count > 0) {
        printf("\nWord List:\n");
        for (int i = 0; i < word_count; i++) {
            printf("%d: %s\n", i + 1, words[i]);
        }
        
        printf("\nWord Lengths:\n");
        for (int i = 0; i < word_count; i++) {
            int len = strlen(words[i]);
            printf("'%s': %d characters\n", words[i], len);
        }
        
        int total_word_chars = 0;
        for (int i = 0; i < word_count; i++) {
            total_word_chars += strlen(words[i]);
        }
        
        if (word_count > 0) {
            double avg_length = (double)total_word_chars / word_count;
            printf("\nAverage word length: %.2f\n", avg_length);
        }
    }
    
    printf("\nCase Analysis:\n");
    int upper_count = 0;
    int lower_count = 0;
    int digit_count = 0;
    int space_count = 0;
    int punct_count = 0;
    
    for (int i = 0; i < input_len; i++) {
        unsigned char c = input[i];
        if (isupper(c)) upper_count++;
        else if (islower(c)) lower_count++;
        else if (isdigit(c)) digit_count++;
        else if (isspace(c)) space_count++;
        else if (ispunct(c)) punct_count++;
    }
    
    printf("Uppercase letters: %d\n", upper_count);
    printf("Lowercase letters: %d\n", lower_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    printf("Punctuation: %d\n", punct_count);
    
    return 0;
}