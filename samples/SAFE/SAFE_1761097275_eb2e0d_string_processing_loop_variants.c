//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT * 2];
    char temp[MAX_INPUT];
    
    printf("Enter text to process: ");
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
        printf("No input provided\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t p_idx = 0;
    size_t i;
    
    for (i = 0; i < len; i++) {
        if (isalpha(input[i])) {
            if (p_idx < sizeof(processed) - 3) {
                processed[p_idx++] = toupper(input[i]);
                processed[p_idx++] = '-';
            }
        }
    }
    
    if (p_idx > 0 && processed[p_idx - 1] == '-') {
        p_idx--;
    }
    processed[p_idx] = '\0';
    
    size_t t_idx = 0;
    i = 0;
    while (i < p_idx) {
        if (t_idx < sizeof(temp) - 1) {
            temp[t_idx++] = processed[i];
        }
        i++;
    }
    temp[t_idx] = '\0';
    
    char reversed[MAX_INPUT];
    size_t r_idx = 0;
    size_t j = t_idx;
    
    do {
        if (j > 0) {
            j--;
            if (r_idx < sizeof(reversed) - 1) {
                reversed[r_idx++] = temp[j];
            }
        }
    } while (j > 0);
    reversed[r_idx] = '\0';
    
    char final[MAX_INPUT * 2];
    size_t f_idx = 0;
    
    for (i = 0; i < r_idx; i++) {
        if (f_idx < sizeof(final) - 2) {
            final[f_idx++] = reversed[i];
            if (i < r_idx - 1 && reversed[i] != '-') {
                final[f_idx++] = '.';
            }
        }
    }
    final[f_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Final: %s\n", final);
    
    return 0;
}