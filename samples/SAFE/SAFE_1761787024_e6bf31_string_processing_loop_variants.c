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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
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
    
    if (j == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    printf("Original: '%s'\n", input);
    printf("Alphanumeric only: '%s'\n", output);
    
    char reversed[MAX_LEN];
    int k = 0;
    int out_len = strlen(output);
    
    while (out_len > 0) {
        reversed[k++] = output[out_len - 1];
        out_len--;
    }
    reversed[k] = '\0';
    
    printf("Reversed: '%s'\n", reversed);
    
    int is_palindrome = 1;
    int left = 0;
    int right = strlen(output) - 1;
    
    while (left < right) {
        if (output[left] != output[right]) {
            is_palindrome = 0;
            break;
        }
        left++;
        right--;
    }
    
    if (is_palindrome) {
        printf("The alphanumeric string is a palindrome\n");
    } else {
        printf("The alphanumeric string is not a palindrome\n");
    }
    
    int char_count[256] = {0};
    int unique_count = 0;
    
    for (i = 0; output[i] != '\0'; i++) {
        unsigned char c = (unsigned char)output[i];
        if (char_count[c] == 0) {
            unique_count++;
        }
        char_count[c]++;
    }
    
    printf("Unique alphanumeric characters: %d\n", unique_count);
    
    return 0;
}