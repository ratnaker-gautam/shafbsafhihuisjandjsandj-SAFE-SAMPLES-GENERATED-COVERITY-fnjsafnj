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
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
    
    if (len >= MAX_LEN) {
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
    int m = j - 1;
    while (m >= 0) {
        reversed[rev_idx++] = processed[m--];
    }
    reversed[rev_idx] = '\0';
    
    char final[MAX_LEN * 2];
    int final_idx = 0;
    int n = 0;
    do {
        if (n < j) {
            final[final_idx++] = processed[n];
        }
        if (n < rev_idx) {
            final[final_idx++] = reversed[n];
        }
        n++;
    } while (n < j || n < rev_idx);
    
    if (final_idx >= sizeof(final)) {
        fprintf(stderr, "Final result too large\n");
        return 1;
    }
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (int p = 0; p < alnum_count; p++) {
        putchar(processed[p]);
    }
    printf("\n");
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Final interleaved: %s\n", final);
    
    return 0;
}