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
    
    printf("\nWord frequency analysis:\n");
    printf("Total words: %d\n", word_count);
    
    int processed[MAX_WORDS] = {0};
    int unique_count = 0;
    
    for (int i = 0; i < word_count; i++) {
        if (processed[i]) {
            continue;
        }
        
        int count = 1;
        for (int j = i + 1; j < word_count; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                count++;
                processed[j] = 1;
            }
        }
        
        unique_count++;
        printf("'%s': %d occurrence(s)\n", words[i], count);
    }
    
    printf("Unique words: %d\n", unique_count);
    
    char longest_word[MAX_WORD_LEN] = "";
    size_t max_len = 0;
    
    for (int i = 0; i < word_count; i++) {
        size_t current_len = strlen(words[i]);
        if (current_len > max_len) {
            max_len = current_len;
            strncpy(longest_word, words[i], MAX_WORD_LEN - 1);
            longest_word[MAX_WORD_LEN - 1] = '\0';
        }
    }
    
    if (max_len > 0) {
        printf("Longest word: '%s' (%zu characters)\n", longest_word, max_len);
    }
    
    int letter_count[26] = {0};
    int total_letters = 0;
    
    for (int i = 0; i < word_count; i++) {
        for (size_t j = 0; j < strlen(words[i]); j++) {
            char c = words[i][j];
            if (isalpha((unsigned char)c)) {
                int index = tolower((unsigned char)c) - 'a';
                if (index >= 0 && index < 26) {
                    letter_count[index]++;
                    total_letters++;
                }
            }
        }
    }
    
    if (total_letters > 0) {
        printf("\nLetter frequency (case-insensitive):\n");
        for (int i = 0; i < 26; i++) {
            if (letter_count[i] > 0) {
                double percentage = (double)letter_count[i] / total_letters * 100.0;
                printf("%c: %d (%.1f%%)\n", 'a' + i, letter_count[i], percentage);
            }
        }
    }
    
    return 0;
}