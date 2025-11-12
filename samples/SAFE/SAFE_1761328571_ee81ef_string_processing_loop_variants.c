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
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    
    int alnum_count = j;
    
    for (k = 0; k < alnum_count; k++) {
        processed[j++] = processed[k];
    }
    
    if (j >= sizeof(processed)) {
        fprintf(stderr, "Processing would exceed buffer\n");
        return 1;
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: ");
    
    int output_len = strlen(processed);
    for (int idx = 0; idx < output_len; idx++) {
        putchar(processed[idx]);
    }
    putchar('\n');
    
    int vowel_count = 0;
    int consonant_count = 0;
    const char *vowels = "aeiouAEIOU";
    
    char *ptr = processed;
    while (*ptr) {
        if (isalpha((unsigned char)*ptr)) {
            int is_vowel = 0;
            for (const char *v = vowels; *v; v++) {
                if (*ptr == *v) {
                    is_vowel = 1;
                    break;
                }
            }
            if (is_vowel) {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        ptr++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    int digit_sum = 0;
    ptr = processed;
    do {
        if (isdigit((unsigned char)*ptr)) {
            digit_sum += *ptr - '0';
        }
        ptr++;
    } while (*ptr);
    
    printf("Digit sum: %d\n", digit_sum);
    
    return 0;
}