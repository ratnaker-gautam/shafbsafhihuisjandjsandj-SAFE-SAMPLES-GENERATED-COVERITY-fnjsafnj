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
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = strlen(processed);
    
    if (rev_len >= sizeof(reversed)) {
        fprintf(stderr, "Processing error\n");
        return 1;
    }
    
    int m = 0;
    int n = rev_len - 1;
    while (n >= 0) {
        reversed[m] = processed[n];
        m++;
        n--;
    }
    reversed[m] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_idx = 0;
    int count = 1;
    char current = reversed[0];
    
    for (int idx = 1; idx <= strlen(reversed); idx++) {
        if (reversed[idx] == current && idx < strlen(reversed)) {
            count++;
        } else {
            if (final_idx < sizeof(final) - 10) {
                final_idx += snprintf(final + final_idx, sizeof(final) - final_idx, "%c%d", current, count);
            }
            current = reversed[idx];
            count = 1;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Final (run-length encoded): %s\n", final);
    
    return 0;
}