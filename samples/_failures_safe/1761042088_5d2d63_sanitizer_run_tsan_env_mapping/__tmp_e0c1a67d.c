//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    
    if (j >= (int)sizeof(processed)) {
        fprintf(stderr, "Processing would exceed buffer\n");
        return 1;
    }
    
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = j;
    
    int m = 0;
    int n = rev_len - 1;
    while (m < rev_len) {
        reversed[m] = processed[n];
        m++;
        n--;
    }
    reversed[rev_len] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_idx = 0;
    
    for (int p = 0; p < rev_len; p++) {
        if (p % 3 == 0) {
            final[final_idx++] = toupper((unsigned char)reversed[p]);
        } else if (p % 3 == 1) {
            final[final_idx++] = tolower((unsigned char)reversed[p]);
        } else {
            final[final_idx++] = reversed[p];
        }
        
        if (final_idx >= (int)sizeof(final) - 1) {
            break;
        }
    }
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Final: %s\n", final);
    
    int digit_count = 0;
    int letter_count = 0;
    
    int idx = 0;
    do {
        if (isdigit((unsigned char)final[idx])) {
            digit_count++;
        } else if (isalpha((unsigned char)final[idx])) {
            letter_count++;
        }
        idx++;
    } while (final[idx] != '\0');
    
    printf("Digits in final: %d\n", digit_count);
    printf("Letters in final: %d\n", letter_count);
    
    return 0;
}