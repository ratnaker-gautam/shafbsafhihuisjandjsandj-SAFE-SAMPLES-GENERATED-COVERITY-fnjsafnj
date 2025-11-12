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
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    int rev_len = strlen(processed);
    
    for (i = rev_len - 1, k = 0; i >= 0; i--, k++) {
        if (k < (int)sizeof(reversed) - 1) {
            reversed[k] = processed[i];
        } else {
            break;
        }
    }
    reversed[k] = '\0';
    
    char final[MAX_LENGTH * 2];
    int final_len = 0;
    int count = 1;
    
    for (i = 0; reversed[i] != '\0'; i++) {
        if (i > 0 && reversed[i] == reversed[i - 1]) {
            count++;
        } else {
            if (i > 0) {
                if (final_len < (int)sizeof(final) - 3) {
                    final[final_len++] = '0' + count;
                }
            }
            count = 1;
            if (final_len < (int)sizeof(final) - 1) {
                final[final_len++] = reversed[i];
            }
        }
    }
    
    if (final_len < (int)sizeof(final) - 1) {
        final[final_len++] = '0' + count;
    }
    final[final_len] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Final: %s\n", final);
    
    return 0;
}