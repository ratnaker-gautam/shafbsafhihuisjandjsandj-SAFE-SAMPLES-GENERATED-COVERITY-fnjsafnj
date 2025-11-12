//DeepSeek-V3 V2.5 Category: Safe ; Style: monolithic_main ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024
#define MAX_WORDS 512

int main(void) {
    char input[MAX_INPUT_LEN];
    char words[MAX_WORDS][64];
    int word_count = 0;
    int i, j, k;
    
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
    
    if (input_len == 0) {
        printf("No input provided\n");
        return 1;
    }
    
    if (input_len >= MAX_INPUT_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    i = 0;
    while (i < input_len && word_count < MAX_WORDS) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace((unsigned char)input[i]) && j < 63) {
            words[word_count][j] = input[i];
            j++;
            i++;
        }
        words[word_count][j] = '\0';
        
        if (j > 0) {
            word_count++;
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    for (i = 0; i < word_count - 1; i++) {
        for (j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[64];
                strncpy(temp, words[j], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[j], words[j + 1], sizeof(words[j]) - 1);
                words[j][sizeof(words[j]) - 1] = '\0';
                strncpy(words[j + 1], temp, sizeof(words[j + 1]) - 1);
                words[j + 1][sizeof(words[j + 1]) - 1] = '\0';
            }
        }
    }
    
    printf("Sorted unique words:\n");
    for (i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    int char_count = 0;
    int letter_count = 0;
    int digit_count = 0;
    
    for (i = 0; i < input_len; i++) {
        char_count++;
        if (isalpha((unsigned char)input[i])) {
            letter_count++;
        } else if (isdigit((unsigned char)input[i])) {
            digit_count++;
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total characters: %d\n", char_count);
    printf("Letters: %d\n", letter_count);
    printf("Digits: %d\n", digit_count);
    printf("Other characters: %d\n", char_count - letter_count - digit_count);
    
    return 0;
}