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
            output[j++] = toupper((unsigned char)input[i]);
        }
    }
    output[j] = '\0';
    
    if (j == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return 1;
    }
    
    int out_len = j;
    char reversed[MAX_LEN];
    
    i = out_len - 1;
    j = 0;
    while (i >= 0) {
        reversed[j++] = output[i];
        i--;
    }
    reversed[j] = '\0';
    
    int is_palindrome = 1;
    int k = 0;
    int l = out_len - 1;
    do {
        if (output[k] != output[l]) {
            is_palindrome = 0;
            break;
        }
        k++;
        l--;
    } while (k < l);
    
    printf("Original: %s\n", input);
    printf("Uppercase letters: %s\n", output);
    printf("Reversed: %s\n", reversed);
    
    if (is_palindrome) {
        printf("The uppercase string is a palindrome\n");
    } else {
        printf("The uppercase string is not a palindrome\n");
    }
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = output;
    while (*ptr) {
        char c = *ptr;
        if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
            vowel_count++;
        } else {
            consonant_count++;
        }
        ptr++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    return 0;
}