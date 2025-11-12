//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256
#define MAX_WORDS 50
#define MAX_WORD_LEN 30

int main() {
    char input[MAX_INPUT_LEN + 1];
    char words[MAX_WORDS][MAX_WORD_LEN + 1];
    int word_count = 0;
    int char_count = 0;
    int digit_count = 0;
    int i, j, k;
    
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
    
    i = 0;
    while (i < input_len) {
        while (i < input_len && isspace((unsigned char)input[i])) {
            i++;
        }
        
        if (i >= input_len) {
            break;
        }
        
        j = 0;
        while (i < input_len && !isspace((unsigned char)input[i]) && j < MAX_WORD_LEN) {
            if (word_count < MAX_WORDS) {
                words[word_count][j] = input[i];
                j++;
            }
            i++;
        }
        
        if (word_count < MAX_WORDS && j > 0) {
            words[word_count][j] = '\0';
            word_count++;
        }
    }
    
    for (i = 0; i < input_len; i++) {
        if (isalpha((unsigned char)input[i])) {
            char_count++;
        } else if (isdigit((unsigned char)input[i])) {
            digit_count++;
        }
    }
    
    printf("Original string: %s\n", input);
    printf("Character count: %d\n", char_count);
    printf("Digit count: %d\n", digit_count);
    printf("Words found: %d\n", word_count);
    
    if (word_count > 0) {
        printf("Words:\n");
        k = 0;
        do {
            printf("  %d: %s\n", k + 1, words[k]);
            k++;
        } while (k < word_count);
    }
    
    printf("Processing words with different loop styles:\n");
    
    if (word_count > 0) {
        printf("Reversed words:\n");
        for (i = 0; i < word_count; i++) {
            size_t word_len = strlen(words[i]);
            if (word_len > 0) {
                printf("  ");
                for (j = word_len - 1; j >= 0; j--) {
                    putchar(words[i][j]);
                }
                putchar('\n');
            }
        }
    }
    
    int total_length = 0;
    i = 0;
    while (i < word_count) {
        total_length += strlen(words[i]);
        i++;
    }
    
    if (word_count > 0) {
        printf("Average word length: %.2f\n", (double)total_length / word_count);
    }
    
    return 0;
}