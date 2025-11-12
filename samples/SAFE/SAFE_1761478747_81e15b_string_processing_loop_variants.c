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
    int rev_idx = 0;
    
    for (i = j - 1; i >= 0; i--) {
        if (rev_idx < sizeof(reversed) - 1) {
            reversed[rev_idx++] = processed[i];
        } else {
            break;
        }
    }
    reversed[rev_idx] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_idx = 0;
    int count = 1;
    
    for (i = 0; i < rev_idx; i++) {
        if (i < rev_idx - 1 && reversed[i] == reversed[i + 1]) {
            count++;
        } else {
            if (final_idx < sizeof(final) - 10) {
                final_idx += snprintf(final + final_idx, sizeof(final) - final_idx, "%d%c", count, reversed[i]);
            } else {
                break;
            }
            count = 1;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Alphanumeric only: ");
    for (i = 0; i < alnum_count; i++) {
        putchar(processed[i]);
    }
    printf("\n");
    printf("Duplicated: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Run-length encoded: %s\n", final);
    
    return 0;
}