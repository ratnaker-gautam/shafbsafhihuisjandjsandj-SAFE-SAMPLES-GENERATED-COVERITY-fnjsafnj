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
    
    printf("Enter a string (max %d characters): ", MAX_LEN - 1);
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    if (len >= MAX_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalpha((unsigned char)input[i])) {
            output[j] = tolower((unsigned char)input[i]);
            j++;
        }
    }
    output[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    int is_palindrome = 1;
    int k = 0;
    int output_len = j;
    
    while (k < output_len / 2) {
        if (output[k] != output[output_len - 1 - k]) {
            is_palindrome = 0;
            break;
        }
        k++;
    }
    
    char reversed[MAX_LEN];
    int m = 0;
    int n = output_len - 1;
    
    do {
        reversed[m] = output[n];
        m++;
        n--;
    } while (n >= 0);
    reversed[m] = '\0';
    
    printf("Original filtered: %s\n", output);
    printf("Reversed: %s\n", reversed);
    
    if (is_palindrome) {
        printf("The filtered string is a palindrome\n");
    } else {
        printf("The filtered string is not a palindrome\n");
    }
    
    int char_count[26] = {0};
    int idx;
    
    for (i = 0; i < output_len; i++) {
        idx = output[i] - 'a';
        if (idx >= 0 && idx < 26) {
            char_count[idx]++;
        }
    }
    
    printf("Character frequencies:\n");
    char ch = 'a';
    do {
        if (char_count[ch - 'a'] > 0) {
            printf("%c: %d\n", ch, char_count[ch - 'a']);
        }
        ch++;
    } while (ch <= 'z');
    
    return 0;
}