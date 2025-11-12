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
    
    len = (int)strlen(input);
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
            processed[j++] = toupper((unsigned char)input[i]);
            processed[j++] = tolower((unsigned char)input[i]);
        } else if (isdigit((unsigned char)input[i])) {
            for (k = 0; k < 3; k++) {
                if (j < sizeof(processed) - 1) {
                    processed[j++] = input[i];
                }
            }
        } else {
            if (j < sizeof(processed) - 1) {
                processed[j++] = input[i];
            }
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
            char lower_c = tolower((unsigned char)c);
            if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                lower_c == 'o' || lower_c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        i++;
    }
    
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    
    int digit_sum = 0;
    for (i = 0; processed[i] != '\0'; i++) {
        if (isdigit((unsigned char)processed[i])) {
            digit_sum += (processed[i] - '0');
        }
    }
    printf("Digit sum: %d\n", digit_sum);
    
    char reversed[MAX_LEN * 2];
    int proc_len = (int)strlen(processed);
    for (i = 0; i < proc_len; i++) {
        reversed[i] = processed[proc_len - 1 - i];
    }
    reversed[proc_len] = '\0';
    printf("Reversed: %s\n", reversed);
    
    return 0;
}