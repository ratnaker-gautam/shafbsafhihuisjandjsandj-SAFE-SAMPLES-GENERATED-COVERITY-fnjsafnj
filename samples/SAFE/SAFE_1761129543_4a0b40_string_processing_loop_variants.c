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
    
    if (len >= MAX_LEN - 1) {
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
    
    char reversed[MAX_LEN * 2];
    int rev_idx = 0;
    int pos = j - 1;
    
    while (pos >= 0) {
        reversed[rev_idx++] = processed[pos--];
    }
    reversed[rev_idx] = '\0';
    
    if (rev_idx >= sizeof(reversed)) {
        fprintf(stderr, "Reversal would exceed buffer\n");
        return 1;
    }
    
    char final[MAX_LEN * 2];
    int final_idx = 0;
    int count = 1;
    
    for (int idx = 0; idx < rev_idx; idx++) {
        final[final_idx++] = reversed[idx];
        
        if (count % 3 == 0 && idx < rev_idx - 1) {
            final[final_idx++] = '-';
        }
        count++;
        
        if (final_idx >= sizeof(final) - 1) {
            fprintf(stderr, "Final output would exceed buffer\n");
            return 1;
        }
    }
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (int idx = 0; idx < alnum_count; idx++) {
        putchar(processed[idx]);
    }
    printf("\n");
    printf("Processed: %s\n", final);
    
    return 0;
}