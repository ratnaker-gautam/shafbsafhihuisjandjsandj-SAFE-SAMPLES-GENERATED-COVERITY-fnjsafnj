//DeepSeek-V3 V2.5 Category: Safe ; Style: cpu_bound ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) return 0;
    }
    return 1;
}

void process_word(char *word) {
    if (word == NULL || *word == '\0') return;
    
    size_t len = strlen(word);
    for (size_t i = 0; i < len; i++) {
        if (isalpha((unsigned char)word[i])) {
            if (islower((unsigned char)word[i])) {
                word[i] = (char)toupper((unsigned char)word[i]);
            } else {
                word[i] = (char)tolower((unsigned char)word[i]);
            }
        }
    }
    
    for (size_t i = 0; i < len / 2; i++) {
        char temp = word[i];
        word[i] = word[len - 1 - i];
        word[len - 1 - i] = temp;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char *words[MAX_WORDS];
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
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    char *token = strtok(input, " \t");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0) {
            words[word_count] = malloc(token_len + 1);
            if (words[word_count] == NULL) {
                fprintf(stderr, "Memory allocation failed\n");
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return 1;
            }
            strcpy(words[word_count], token);
            word_count++;
        }
        token = strtok(NULL, " \t");
    }
    
    for (int i = 0; i < word_count; i++) {
        process_word(words[i]);
    }
    
    printf("Processed text: ");
    for (int i = 0; i < word_count; i++) {
        printf("%s", words[i]);
        if (i < word_count - 1) {
            printf(" ");
        }
        free(words[i]);
    }
    printf("\n");
    
    return 0;
}