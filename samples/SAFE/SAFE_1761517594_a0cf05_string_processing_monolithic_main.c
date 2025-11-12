//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] != '\0') {
            char_count++;
        }
    }
    
    for (size_t i = 0; i < input_len; i++) {
        if (input[i] == '\n') {
            line_count++;
        }
    }
    if (input_len > 0) {
        line_count++;
    }
    
    printf("\nText analysis results:\n");
    printf("Characters: %d\n", char_count);
    printf("Words: %d\n", word_count);
    printf("Lines: %d\n", line_count);
    
    printf("\nWords found:\n");
    for (int i = 0; i < word_count; i++) {
        printf("%d: %s\n", i + 1, words[i]);
    }
    
    printf("\nProcessed text (uppercase):\n");
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        if (isalpha((unsigned char)c)) {
            putchar(toupper((unsigned char)c));
        } else {
            putchar(c);
        }
    }
    printf("\n");
    
    return 0;
}