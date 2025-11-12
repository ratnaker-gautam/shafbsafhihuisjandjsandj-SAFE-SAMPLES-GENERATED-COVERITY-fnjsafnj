//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN];
    int i, j, k;
    int len;
    
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            output[j++] = input[i];
        }
    }
    output[j] = '\0';
    
    printf("Alphanumeric characters: %s\n", output);
    
    int word_count = 0;
    int in_word = 0;
    i = 0;
    while (input[i] != '\0') {
        if (isalnum((unsigned char)input[i])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        i++;
    }
    printf("Word count: %d\n", word_count);
    
    char reversed[MAX_LEN];
    k = 0;
    for (i = len - 1; i >= 0; i--) {
        reversed[k++] = input[i];
    }
    reversed[k] = '\0';
    printf("Reversed string: %s\n", reversed);
    
    char vowels_only[MAX_LEN];
    j = 0;
    for (i = 0; input[i] != '\0'; i++) {
        char c = tolower((unsigned char)input[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowels_only[j++] = input[i];
        }
    }
    vowels_only[j] = '\0';
    printf("Vowels only: %s\n", vowels_only);
    
    int digit_sum = 0;
    i = 0;
    do {
        if (isdigit((unsigned char)input[i])) {
            digit_sum += input[i] - '0';
        }
        i++;
    } while (input[i] != '\0');
    printf("Sum of digits: %d\n", digit_sum);
    
    return 0;
}