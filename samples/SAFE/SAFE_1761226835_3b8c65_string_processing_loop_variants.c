//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
            processed[j++] = input[i];
        }
    }
    
    int alnum_count = j;
    
    for (k = 0; k < alnum_count; k++) {
        processed[j++] = processed[k];
    }
    processed[j] = '\0';
    
    char reversed[MAX_LEN * 2];
    int rev_len = strlen(processed);
    
    for (i = 0; i < rev_len; i++) {
        reversed[i] = processed[rev_len - 1 - i];
    }
    reversed[rev_len] = '\0';
    
    int is_palindrome = 1;
    for (i = 0; i < rev_len / 2; i++) {
        if (processed[i] != reversed[i]) {
            is_palindrome = 0;
            break;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (i = 0; i < alnum_count; i++) {
        putchar(processed[i]);
    }
    printf("\n");
    printf("Duplicated: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Is palindrome: %s\n", is_palindrome ? "yes" : "no");
    
    int digit_sum = 0;
    for (i = 0; i < alnum_count; i++) {
        if (isdigit((unsigned char)processed[i])) {
            digit_sum += (processed[i] - '0');
        }
    }
    printf("Digit sum: %d\n", digit_sum);
    
    return 0;
}