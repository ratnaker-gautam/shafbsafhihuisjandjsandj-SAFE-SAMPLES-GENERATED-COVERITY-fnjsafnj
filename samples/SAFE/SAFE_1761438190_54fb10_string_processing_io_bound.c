//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
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
    
    printf("\nWord count: %d\n", word_count);
    printf("Words in reverse order:\n");
    
    for (int i = word_count - 1; i >= 0; i--) {
        if (i < word_count) {
            printf("%s\n", words[i]);
        }
    }
    
    printf("\nWord lengths:\n");
    for (int i = 0; i < word_count; i++) {
        size_t len = strlen(words[i]);
        if (len > 0) {
            printf("'%s': %zu characters\n", words[i], len);
        }
    }
    
    int uppercase_count = 0;
    int lowercase_count = 0;
    int digit_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        unsigned char c = input[i];
        if (isupper(c)) {
            uppercase_count++;
        } else if (islower(c)) {
            lowercase_count++;
        } else if (isdigit(c)) {
            digit_count++;
        }
    }
    
    printf("\nCharacter statistics:\n");
    printf("Uppercase letters: %d\n", uppercase_count);
    printf("Lowercase letters: %d\n", lowercase_count);
    printf("Digits: %d\n", digit_count);
    printf("Total characters: %zu\n", input_len);
    
    return 0;
}