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
    
    if (j >= sizeof(processed)) {
        fprintf(stderr, "Processing would exceed buffer\n");
        return 1;
    }
    
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = strlen(processed);
    
    if (rev_len >= sizeof(reversed)) {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    int m, n;
    for (m = rev_len - 1, n = 0; m >= 0; m--, n++) {
        reversed[n] = processed[m];
    }
    reversed[n] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_len = 0;
    
    int p = 0;
    while (p < rev_len) {
        int count = 1;
        while (p + count < rev_len && processed[p] == processed[p + count]) {
            count++;
        }
        
        if (final_len + 3 >= sizeof(final)) {
            fprintf(stderr, "Final buffer overflow\n");
            return 1;
        }
        
        final[final_len++] = processed[p];
        if (count > 1) {
            final[final_len++] = 'x';
            final[final_len++] = '0' + (count % 10);
        }
        p += count;
    }
    final[final_len] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (int q = 0; q < alnum_count; q++) {
        putchar(processed[q]);
    }
    printf("\n");
    printf("Duplicated: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Compressed: %s\n", final);
    
    return 0;
}