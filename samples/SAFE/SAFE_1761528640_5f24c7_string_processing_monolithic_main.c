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
    int word_length = 0;
    
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (word_count >= MAX_WORDS) {
                fprintf(stderr, "Too many words\n");
                return 1;
            }
            if (word_length >= 63) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            words[word_count][word_length++] = tolower((unsigned char)input[i]);
        } else {
            if (word_length > 0) {
                words[word_count][word_length] = '\0';
                word_count++;
                word_length = 0;
            }
        }
    }
    
    if (word_count == 0) {
        printf("No words found in input\n");
        return 1;
    }
    
    for (int i = 0; i < word_count - 1; i++) {
        for (int j = 0; j < word_count - i - 1; j++) {
            if (strcmp(words[j], words[j + 1]) > 0) {
                char temp[64];
                if (strlen(words[j]) < 64 && strlen(words[j + 1]) < 64) {
                    strcpy(temp, words[j]);
                    strcpy(words[j], words[j + 1]);
                    strcpy(words[j + 1], temp);
                }
            }
        }
    }
    
    printf("Sorted unique words:\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            if (printf("%s\n", words[i]) < 0) {
                fprintf(stderr, "Output error\n");
                return 1;
            }
        }
    }
    
    int char_count = 0;
    int letter_count = 0;
    int digit_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
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
    printf("Words processed: %d\n", word_count);
    
    return 0;
}