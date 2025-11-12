//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

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
        if (j >= (int)sizeof(processed) - 1) {
            break;
        }
        if (isalpha((unsigned char)input[i])) {
            if (j < (int)sizeof(processed) - 2) {
                processed[j++] = toupper((unsigned char)input[i]);
                processed[j++] = tolower((unsigned char)input[i]);
            }
        } else if (isdigit((unsigned char)input[i])) {
            for (k = 0; k < 3; k++) {
                if (j < (int)sizeof(processed) - 1) {
                    processed[j++] = input[i];
                } else {
                    break;
                }
            }
        } else {
            processed[j++] = '*';
        }
        i++;
    }
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    size_t proc_len = strlen(processed);
    for (i = (int)proc_len - 1, k = 0; i >= 0; i--, k++) {
        if (k < (int)sizeof(reversed) - 1) {
            reversed[k] = processed[i];
        } else {
            break;
        }
    }
    reversed[k] = '\0';
    
    int vowel_count = 0;
    int consonant_count = 0;
    char *ptr = processed;
    while (*ptr) {
        char c = *ptr;
        if (isalpha((unsigned char)c)) {
            char lower_c = tolower((unsigned char)c);
            if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                lower_c == 'o' || lower_c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        }
        ptr++;
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Vowels: %d, Consonants: %d\n", vowel_count, consonant_count);
    
    int digit_sum = 0;
    for (i = 0; i < (int)len; i++) {
        if (isdigit((unsigned char)input[i])) {
            int digit = input[i] - '0';
            if (digit_sum <= INT_MAX - digit) {
                digit_sum += digit;
            } else {
                digit_sum = INT_MAX;
                break;
            }
        }
    }
    printf("Digit sum: %d\n", digit_sum);
    
    return 0;
}