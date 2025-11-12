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
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = input[i];
        }
    }
    
    int alnum_count = j;
    
    for (k = 0; k < alnum_count; k++) {
        processed[j++] = processed[k];
    }
    processed[j] = '\0';
    
    char reversed[MAX_LEN * 2];
    int rev_len = j;
    
    int m = rev_len - 1;
    int n = 0;
    while (m >= 0) {
        reversed[n++] = processed[m--];
    }
    reversed[n] = '\0';
    
    char final[MAX_LEN * 2];
    int final_len = 0;
    
    int p = 0;
    do {
        if (p < rev_len) {
            final[final_len++] = reversed[p];
        }
        p++;
    } while (p < rev_len);
    
    int q = 0;
    while (q < alnum_count) {
        final[final_len++] = input[q];
        q++;
    }
    final[final_len] = '\0';
    
    if (final_len >= (int)sizeof(final)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", final);
    
    int digit_count = 0;
    for (int r = 0; r < final_len; r++) {
        if (isdigit((unsigned char)final[r])) {
            digit_count++;
        }
    }
    
    printf("Digits in result: %d\n", digit_count);
    
    return 0;
}