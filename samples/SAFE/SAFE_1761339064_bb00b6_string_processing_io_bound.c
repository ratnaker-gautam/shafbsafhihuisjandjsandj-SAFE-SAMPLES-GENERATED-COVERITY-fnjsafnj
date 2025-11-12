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
    int char_count = 0;
    int line_count = 0;
    
    printf("Enter text (max %d characters):\n", MAX_INPUT_LEN - 1);
    
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
    
    char_count = input_len;
    line_count = 1;
    
    char *token = strtok(input, " \t\n");
    while (token != NULL && word_count < MAX_WORD_COUNT) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len < MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN - 1);
            words[word_count][MAX_WORD_LEN - 1] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
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
            printf("%s: %zu characters\n", words[i], strlen(words[i]));
        }
    }
    
    int letter_count = 0;
    int digit_count = 0;
    int space_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        if (isalpha(input[i])) {
            letter_count++;
        } else if (isdigit(input[i])) {
            digit_count++;
        } else if (isspace(input[i])) {
            space_count++;
        }
    }
    
    printf("\nCharacter Types:\n");
    printf("Letters: %d\n", letter_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    
    return 0;
}