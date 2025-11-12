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
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    
    int alnum_count = j;
    processed[j] = '\0';
    
    printf("Alphanumeric characters (uppercase): %s\n", processed);
    printf("Count: %d\n", alnum_count);
    
    char reversed[MAX_LENGTH * 2];
    k = 0;
    int pos = alnum_count - 1;
    while (pos >= 0) {
        reversed[k++] = processed[pos];
        pos--;
    }
    reversed[k] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    char vowels_only[MAX_LENGTH * 2];
    int vowel_count = 0;
    int idx = 0;
    do {
        if (idx < alnum_count) {
            char c = processed[idx];
            if (c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U') {
                vowels_only[vowel_count++] = c;
            }
        }
        idx++;
    } while (idx <= alnum_count);
    vowels_only[vowel_count] = '\0';
    
    printf("Vowels only: %s\n", vowels_only);
    printf("Vowel count: %d\n", vowel_count);
    
    int digit_sum = 0;
    for (int m = 0; m < alnum_count; m++) {
        if (isdigit((unsigned char)processed[m])) {
            int digit = processed[m] - '0';
            if (digit_sum <= INT_MAX - digit) {
                digit_sum += digit;
            }
        }
    }
    
    printf("Sum of digits: %d\n", digit_sum);
    
    char no_repeats[MAX_LENGTH * 2];
    int nr_count = 0;
    int prev_char = -1;
    
    for (int n = 0; n < alnum_count; n++) {
        if (processed[n] != prev_char) {
            no_repeats[nr_count++] = processed[n];
            prev_char = processed[n];
        }
    }
    no_repeats[nr_count] = '\0';
    
    printf("Without consecutive duplicates: %s\n", no_repeats);
    
    return 0;
}