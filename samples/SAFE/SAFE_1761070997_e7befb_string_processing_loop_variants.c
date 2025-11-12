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
        processed[j++] = processed[alnum_count - k - 1];
    }
    processed[j] = '\0';
    
    char temp;
    int palindrome_len = j;
    for (i = 0; i < palindrome_len / 2; i++) {
        temp = processed[i];
        processed[i] = processed[palindrome_len - i - 1];
        processed[palindrome_len - i - 1] = temp;
    }
    
    char final[MAX_LENGTH * 2];
    int final_idx = 0;
    int count = 1;
    
    i = 0;
    while (i < palindrome_len) {
        final[final_idx++] = processed[i];
        
        count = 1;
        while (i + count < palindrome_len && processed[i + count] == processed[i]) {
            count++;
        }
        
        if (count > 1) {
            char num_str[10];
            int written = snprintf(num_str, sizeof(num_str), "%d", count);
            if (written < 0 || written >= (int)sizeof(num_str)) {
                fprintf(stderr, "Number formatting error\n");
                return 1;
            }
            
            for (k = 0; k < written && final_idx < (int)sizeof(final) - 1; k++) {
                final[final_idx++] = num_str[k];
            }
        }
        
        i += count;
        
        if (final_idx >= (int)sizeof(final) - 10) {
            break;
        }
    }
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", final);
    
    return 0;
}