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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
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
    int mid = j / 2;
    for (k = 0; k < mid; k++) {
        temp = processed[k];
        processed[k] = processed[j - k - 1];
        processed[j - k - 1] = temp;
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    int digit_count = 0;
    for (k = 0; k < len; k++) {
        if (isdigit((unsigned char)input[k])) {
            digit_count++;
        }
    }
    
    printf("Digits in original: %d\n", digit_count);
    
    int vowel_count = 0;
    k = 0;
    do {
        if (k < len) {
            char c = tolower((unsigned char)input[k]);
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            }
        }
        k++;
    } while (k < len);
    
    printf("Vowels in original: %d\n", vowel_count);
    
    return 0;
}