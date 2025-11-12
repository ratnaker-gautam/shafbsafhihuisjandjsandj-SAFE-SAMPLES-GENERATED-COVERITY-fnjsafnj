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
    
    printf("Enter a string (max %d chars): ", MAX_LENGTH - 1);
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
        processed[j++] = processed[k];
    }
    
    if (j >= sizeof(processed)) {
        fprintf(stderr, "Processing would exceed buffer\n");
        return 1;
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = processed;
    do {
        char c = *ptr;
        if (c == '\0') break;
        
        switch (c) {
            case 'A': case 'E': case 'I': case 'O': case 'U':
                vowel_count++;
                break;
            default:
                if (isalpha((unsigned char)c)) {
                    consonant_count++;
                }
                break;
        }
        ptr++;
    } while (*ptr != '\0');
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    int palindrome = 1;
    int left = 0;
    int right = alpha_count - 1;
    for (; left < right; left++, right--) {
        if (processed[left] != processed[right]) {
            palindrome = 0;
            break;
        }
    }
    
    printf("Palindrome check (first half): %s\n", palindrome ? "Yes" : "No");
    
    return 0;
}