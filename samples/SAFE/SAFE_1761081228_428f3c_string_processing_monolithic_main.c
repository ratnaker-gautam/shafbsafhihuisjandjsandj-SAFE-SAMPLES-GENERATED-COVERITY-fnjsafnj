//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: monolithic_main ; Variation: string_processing
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
    char temp_word[MAX_WORD_LEN];
    int temp_len = 0;
    
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
    
    for (size_t i = 0; i <= input_len; i++) {
        if (i < input_len && isalpha((unsigned char)input[i])) {
            if (temp_len < MAX_WORD_LEN - 1) {
                temp_word[temp_len++] = tolower((unsigned char)input[i]);
            }
        } else {
            if (temp_len > 0) {
                temp_word[temp_len] = '\0';
                if (word_count < MAX_WORDS) {
                    strncpy(words[word_count], temp_word, MAX_WORD_LEN - 1);
                    words[word_count][MAX_WORD_LEN - 1] = '\0';
                    word_count++;
                }
                temp_len = 0;
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
                char temp[MAX_WORD_LEN];
                strncpy(temp, words[j], sizeof(temp) - 1);
                temp[sizeof(temp) - 1] = '\0';
                strncpy(words[j], words[j + 1], MAX_WORD_LEN - 1);
                words[j][MAX_WORD_LEN - 1] = '\0';
                strncpy(words[j + 1], temp, MAX_WORD_LEN - 1);
                words[j + 1][MAX_WORD_LEN - 1] = '\0';
            }
        }
    }
    
    printf("Sorted unique words:\n");
    for (int i = 0; i < word_count; i++) {
        if (i == 0 || strcmp(words[i], words[i - 1]) != 0) {
            printf("%s\n", words[i]);
        }
    }
    
    int char_count = 0;
    int letter_count = 0;
    int digit_count = 0;
    int space_count = 0;
    
    for (size_t i = 0; i < input_len; i++) {
        char c = input[i];
        char_count++;
        if (isalpha((unsigned char)c)) {
            letter_count++;
        } else if (isdigit((unsigned char)c)) {
            digit_count++;
        } else if (isspace((unsigned char)c)) {
            space_count++;
        }
    }
    
    printf("\nStatistics:\n");
    printf("Total characters: %d\n", char_count);
    printf("Letters: %d\n", letter_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    printf("Words processed: %d\n", word_count);
    
    return 0;
}