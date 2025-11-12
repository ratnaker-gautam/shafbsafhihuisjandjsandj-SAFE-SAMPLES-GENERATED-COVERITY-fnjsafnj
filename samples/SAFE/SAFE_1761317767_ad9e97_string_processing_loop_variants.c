//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 50

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int digit_count = 0;
    int space_count = 0;
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int i = 0;
    while (input[i] != '\0') {
        if (isalpha(input[i])) {
            char_count++;
        } else if (isdigit(input[i])) {
            digit_count++;
        } else if (isspace(input[i])) {
            space_count++;
        }
        i++;
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
    
    printf("Input analysis:\n");
    printf("Total characters: %zu\n", input_len);
    printf("Letters: %d\n", char_count);
    printf("Digits: %d\n", digit_count);
    printf("Spaces: %d\n", space_count);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Words (reversed):\n");
        for (int j = word_count - 1; j >= 0; j--) {
            size_t word_len = strlen(words[j]);
            char reversed[MAX_WORD_LEN + 1];
            
            if (word_len > 0) {
                int k = 0;
                int m = word_len - 1;
                do {
                    reversed[k] = words[j][m];
                    k++;
                    m--;
                } while (m >= 0);
                reversed[k] = '\0';
                printf("  %s -> %s\n", words[j], reversed);
            }
        }
    }
    
    printf("Character frequency (letters only):\n");
    for (char c = 'a'; c <= 'z'; c++) {
        int count = 0;
        int idx = 0;
        while (input[idx] != '\0') {
            if (tolower(input[idx]) == c) {
                count++;
            }
            idx++;
        }
        if (count > 0) {
            printf("  %c: %d\n", c, count);
        }
    }
    
    return 0;
}