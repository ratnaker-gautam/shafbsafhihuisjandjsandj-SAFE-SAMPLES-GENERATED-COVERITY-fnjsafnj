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
        if (isalpha((unsigned char)input[i])) {
            char c = tolower((unsigned char)input[i]);
            processed[j++] = c;
            processed[j++] = c;
        } else if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            for (k = 0; k < digit && j < sizeof(processed) - 1; k++) {
                processed[j++] = '#';
            }
        } else if (isspace((unsigned char)input[i])) {
            processed[j++] = '_';
        } else {
            processed[j++] = '?';
        }
        
        if (j >= sizeof(processed) - 1) {
            break;
        }
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    int vowel_count = 0;
    int consonant_count = 0;
    i = 0;
    while (processed[i] != '\0') {
        char c = processed[i];
        if (isalpha((unsigned char)c)) {
            if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        i++;
    }
    
    printf("Vowels in processed: %d\n", vowel_count);
    printf("Consonants in processed: %d\n", consonant_count);
    
    char reversed[MAX_LEN * 2];
    int proc_len = strlen(processed);
    for (i = proc_len - 1, k = 0; i >= 0; i--, k++) {
        reversed[k] = processed[i];
    }
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int palindrome = 1;
    for (i = 0; i < proc_len / 2; i++) {
        if (processed[i] != processed[proc_len - 1 - i]) {
            palindrome = 0;
            break;
        }
    }
    
    printf("Is palindrome: %s\n", palindrome ? "yes" : "no");
    
    return 0;
}