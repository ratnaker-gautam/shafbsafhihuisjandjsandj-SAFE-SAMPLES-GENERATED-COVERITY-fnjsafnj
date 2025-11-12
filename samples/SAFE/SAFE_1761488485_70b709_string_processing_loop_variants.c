//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    int i, j, len;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string\n");
        return 1;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            output[j] = input[i];
            j++;
        }
    }
    output[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphanumeric characters found\n");
        return 1;
    }
    
    int output_len = j;
    char temp;
    int k = output_len - 1;
    
    i = 0;
    while (i < k) {
        temp = output[i];
        output[i] = output[k];
        output[k] = temp;
        i++;
        k--;
    }
    
    int word_count = 0;
    int in_word = 0;
    
    for (i = 0; i < output_len; i++) {
        if (isalpha((unsigned char)output[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Filtered: %s\n", output);
    printf("Reversed: %s\n", output);
    printf("Word count: %d\n", word_count);
    
    int digit_sum = 0;
    i = 0;
    do {
        if (isdigit((unsigned char)output[i])) {
            digit_sum += output[i] - '0';
        }
        i++;
    } while (i < output_len);
    
    printf("Digit sum: %d\n", digit_sum);
    
    return 0;
}