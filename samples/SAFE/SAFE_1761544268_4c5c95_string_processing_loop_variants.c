//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t p_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        char current = input[i];
        
        if (p_idx >= sizeof(processed) - 3) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        processed[p_idx++] = current;
        
        if (isalpha(current)) {
            if (p_idx >= sizeof(processed) - 3) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[p_idx++] = toupper(current);
        }
        
        i++;
    }
    
    processed[p_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t count = 0;
    for (size_t j = 0; j < p_idx; j++) {
        if (isalpha(processed[j])) {
            count++;
        }
    }
    printf("Total alphabetic characters in output: %zu\n", count);
    
    char reversed[MAX_LEN * 2];
    size_t r_idx = 0;
    size_t k = p_idx;
    
    do {
        if (k == 0) break;
        k--;
        if (r_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversal buffer overflow\n");
            return 1;
        }
        reversed[r_idx++] = processed[k];
    } while (k > 0);
    
    reversed[r_idx] = '\0';
    printf("Reversed: %s\n", reversed);
    
    return 0;
}