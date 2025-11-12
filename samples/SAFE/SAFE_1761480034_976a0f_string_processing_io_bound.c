//DeepSeek-V3 V2.5 Category: Safe ; Style: io_bound ; Variation: string_processing
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
    
    printf("Enter text (max %d characters): ", MAX_INPUT_LEN);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (len > MAX_INPUT_LEN) {
        printf("Input too long\n");
        return 1;
    }
    
    char temp[MAX_INPUT_LEN + 1];
    strncpy(temp, input, sizeof(temp) - 1);
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, " \t\n\r");
    while (token != NULL && word_count < MAX_WORDS) {
        size_t token_len = strlen(token);
        if (token_len > 0 && token_len <= MAX_WORD_LEN) {
            strncpy(words[word_count], token, MAX_WORD_LEN);
            words[word_count][MAX_WORD_LEN] = '\0';
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    
    if (word_count == 0) {
        printf("No valid words found\n");
        return 1;
    }
    
    printf("\nProcessing %d words:\n", word_count);
    for (int i = 0; i < word_count; i++) {
        printf("Word %d: ", i + 1);
        
        int letter_count = 0;
        int digit_count = 0;
        int upper_count = 0;
        int lower_count = 0;
        
        for (size_t j = 0; j < strlen(words[i]); j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                letter_count++;
                if (isupper((unsigned char)c)) {
                    upper_count++;
                } else {
                    lower_count++;
                }
            } else if (isdigit((unsigned char)c)) {
                digit_count++;
            }
        }
        
        printf("'%s' - Letters: %d, Digits: %d, Uppercase: %d, Lowercase: %d\n",
               words[i], letter_count, digit_count, upper_count, lower_count);
    }
    
    printf("\nWord statistics:\n");
    int total_letters = 0;
    int total_digits = 0;
    int shortest_len = MAX_WORD_LEN;
    int longest_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        int len = strlen(words[i]);
        if (len < shortest_len) shortest_len = len;
        if (len > longest_len) longest_len = len;
        
        for (size_t j = 0; j < strlen(words[i]); j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                total_letters++;
            } else if (isdigit((unsigned char)c)) {
                total_digits++;
            }
        }
    }
    
    printf("Total words: %d\n", word_count);
    printf("Total letters: %d\n", total_letters);
    printf("Total digits: %d\n", total_digits);
    printf("Shortest word length: %d\n", shortest_len);
    printf("Longest word length: %d\n", longest_len);
    
    return 0;
}