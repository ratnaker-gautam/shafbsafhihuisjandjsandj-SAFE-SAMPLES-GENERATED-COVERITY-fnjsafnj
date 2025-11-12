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
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
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
        i++;
    }
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t proc_len = strlen(processed);
    int vowel_count = 0;
    int consonant_count = 0;
    int digit_count = 0;
    int other_count = 0;
    
    for (i = 0; i < proc_len; i++) {
        char c = processed[i];
        if (isalpha((unsigned char)c)) {
            char lower_c = tolower((unsigned char)c);
            if (lower_c == 'a' || lower_c == 'e' || lower_c == 'i' || 
                lower_c == 'o' || lower_c == 'u') {
                vowel_count++;
            } else {
                consonant_count++;
            }
        } else if (isdigit((unsigned char)c)) {
            digit_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Statistics:\n");
    printf("Vowels: %d\n", vowel_count);
    printf("Consonants: %d\n", consonant_count);
    printf("Digits: %d\n", digit_count);
    printf("Other characters: %d\n", other_count);
    
    char reversed[MAX_LENGTH * 2];
    size_t rev_len = proc_len;
    for (i = 0; i < rev_len; i++) {
        reversed[i] = processed[rev_len - 1 - i];
    }
    reversed[rev_len] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}