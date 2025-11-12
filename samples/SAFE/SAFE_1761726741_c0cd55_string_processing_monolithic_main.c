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
    
    if (input_len >= sizeof(input) - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *ptr = input;
    while (*ptr != '\0' && word_count < MAX_WORDS) {
        while (*ptr != '\0' && isspace((unsigned char)*ptr)) {
            ptr++;
        }
        
        if (*ptr == '\0') {
            break;
        }
        
        char *word_start = ptr;
        size_t word_len = 0;
        
        while (*ptr != '\0' && !isspace((unsigned char)*ptr) && word_len < MAX_WORD_LEN - 1) {
            ptr++;
            word_len++;
        }
        
        if (word_len >= MAX_WORD_LEN - 1) {
            fprintf(stderr, "Word too long\n");
            return 1;
        }
        
        if (word_len > 0) {
            strncpy(words[word_count], word_start, word_len);
            words[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found\n");
        return 1;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[j], words[j + 1], sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
                strncpy(words[j + 1], temp, sizeof(words[j + 1]) - 1);
                words[j + 1][sizeof(words[j + 1]) - 1] = '\0';
            }
        }
    }
    
    printf("Sorted words:\n");
    for (int i = 0; i < word_count; i++) {
        if (printf("%s\n", words[i]) < 0) {
            fprintf(stderr, "Error writing output\n");
            return 1;
        }
    }
    
    int char_count = 0;
    int letter_count = 0;
    int digit_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        char_count++;
        if (isalpha((unsigned char)c)) {
            letter_count++;
        } else if (isdigit((unsigned char)c)) {
            digit_count++;
        }
    }
    
    printf("Statistics:\n");
    printf("Total characters: %d\n", char_count);
    printf("Letters: %d\n", letter_count);
    printf("Digits: %d\n", digit_count);
    printf("Words: %d\n", word_count);
    
    return 0;
}