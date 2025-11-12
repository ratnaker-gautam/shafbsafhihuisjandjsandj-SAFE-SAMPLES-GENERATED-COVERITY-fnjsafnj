//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1000

int is_valid_word(const char *word) {
    if (word == NULL || strlen(word) == 0 || strlen(word) >= MAX_WORD_LEN) {
        return 0;
    }
    for (size_t i = 0; i < strlen(word); i++) {
        if (!isalpha(word[i])) {
            return 0;
        }
    }
    return 1;
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

int count_vowels(const char *word) {
    if (word == NULL) return 0;
    int count = 0;
    for (size_t i = 0; i < strlen(word); i++) {
        char c = tolower(word[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

void process_word(const char *word) {
    if (!is_valid_word(word)) {
        printf("Invalid word: %s\n", word);
        return;
    }
    
    char temp[MAX_WORD_LEN];
    if (strlen(word) >= MAX_WORD_LEN) {
        printf("Word too long: %s\n", word);
        return;
    }
    strncpy(temp, word, MAX_WORD_LEN - 1);
    temp[MAX_WORD_LEN - 1] = '\0';
    
    to_lowercase(temp);
    int vowel_count = count_vowels(temp);
    
    printf("Word: %s, Length: %zu, Vowels: %d\n", 
           temp, strlen(temp), vowel_count);
}

int read_input(char *buffer, size_t buf_size) {
    if (buffer == NULL || buf_size == 0) {
        return 0;
    }
    
    printf("Enter text (max %zu characters): ", buf_size - 1);
    if (fgets(buffer, buf_size, stdin) == NULL) {
        return 0;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return 1;
}

void tokenize_and_process(const char *input) {
    if (input == NULL) return;
    
    char copy[MAX_LINE_LEN];
    if (strlen(input) >= MAX_LINE_LEN) {
        printf("Input too long\n");
        return;
    }
    strncpy(copy, input, MAX_LINE_LEN - 1);
    copy[MAX_LINE_LEN - 1] = '\0';
    
    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        process_word(token);
        token = strtok(NULL, " \t\n\r");
    }
}

int main(void) {
    char input[MAX_LINE_LEN];
    
    if (!read_input(input, sizeof(input))) {
        printf("Error reading input\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    printf("\nProcessing words:\n");
    tokenize_and_process(input);
    
    return 0;
}