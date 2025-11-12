//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, k;
    
    printf("Enter a string to process: ");
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
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
        i++;
    }
    
    int alpha_count = j;
    
    for (k = 0; k < alpha_count; k++) {
        processed[j++] = processed[alpha_count - k - 1];
    }
    processed[j] = '\0';
    
    char temp;
    int palindrome_len = j;
    for (i = 0, k = palindrome_len - 1; i < k; i++, k--) {
        temp = processed[i];
        processed[i] = processed[k];
        processed[k] = temp;
    }
    
    int vowel_count = 0;
    int consonant_count = 0;
    i = 0;
    do {
        char c = processed[i];
        if (isalpha((unsigned char)c)) {
            if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        i++;
    } while (i < palindrome_len);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Length: %d\n", palindrome_len);
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    int digit_sum = 0;
    for (i = 0; i < len; i++) {
        if (isdigit((unsigned char)input[i])) {
            digit_sum += input[i] - '0';
        }
    }
    printf("Digit sum in original: %d\n", digit_sum);
    
    return 0;
}