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
    
    k = 0;
    while (k < alnum_count) {
        processed[j++] = processed[k++];
    }
    
    processed[j] = '\0';
    
    int total_len = j;
    char reversed[MAX_LEN * 2];
    
    int m = total_len - 1;
    int n = 0;
    while (m >= 0) {
        reversed[n++] = processed[m--];
    }
    reversed[n] = '\0';
    
    char final[MAX_LEN * 2];
    int p = 0;
    int q = 0;
    
    do {
        if (p < total_len) {
            final[q++] = processed[p];
        }
        if (p < n) {
            final[q++] = reversed[p];
        }
        p++;
    } while (p < total_len || p < n);
    
    final[q] = '\0';
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (int r = 0; r < alnum_count; r++) {
        putchar(processed[r]);
    }
    printf("\n");
    printf("Final processed: %s\n", final);
    
    return 0;
}