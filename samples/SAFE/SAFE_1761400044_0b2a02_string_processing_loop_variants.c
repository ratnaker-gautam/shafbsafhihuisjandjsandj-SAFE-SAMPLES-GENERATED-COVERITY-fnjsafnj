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
        printf("Empty input string\n");
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
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = strlen(processed);
    
    if (rev_len >= sizeof(reversed)) {
        fprintf(stderr, "String too long for reversal\n");
        return 1;
    }
    
    int m = 0;
    int n = rev_len - 1;
    while (m < rev_len) {
        reversed[m] = processed[n];
        m++;
        n--;
    }
    reversed[rev_len] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_len = 0;
    int count = 1;
    char current = reversed[0];
    
    for (int idx = 1; idx <= rev_len; idx++) {
        if (idx < rev_len && reversed[idx] == current) {
            count++;
        } else {
            if (final_len + 3 >= sizeof(final)) {
                fprintf(stderr, "Final output too long\n");
                return 1;
            }
            
            final[final_len++] = current;
            if (count > 1) {
                char num_str[10];
                int num_len = snprintf(num_str, sizeof(num_str), "%d", count);
                if (final_len + num_len >= sizeof(final)) {
                    fprintf(stderr, "Final output too long for count\n");
                    return 1;
                }
                for (int c = 0; c < num_len; c++) {
                    final[final_len++] = num_str[c];
                }
            }
            
            if (idx < rev_len) {
                current = reversed[idx];
                count = 1;
            }
        }
    }
    
    final[final_len] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", final);
    
    return 0;
}