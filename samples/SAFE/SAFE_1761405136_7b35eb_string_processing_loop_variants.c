//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    char temp[MAX_LEN];
    
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
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t p_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0) {
            if (p_idx + word_len * 2 >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (size_t j = 0; j < word_len; j++) {
                processed[p_idx++] = input[word_start + j];
                processed[p_idx++] = input[word_start + j];
            }
            
            if (i < len) {
                processed[p_idx++] = input[i++];
            }
        } else {
            i++;
        }
    }
    
    processed[p_idx] = '\0';
    
    size_t temp_idx = 0;
    for (size_t k = 0; k < p_idx; k++) {
        if (temp_idx >= sizeof(temp) - 1) {
            fprintf(stderr, "Temp buffer overflow\n");
            return 1;
        }
        temp[temp_idx++] = toupper((unsigned char)processed[k]);
    }
    temp[temp_idx] = '\0';
    
    char final[MAX_LEN];
    size_t final_idx = 0;
    size_t m = 0;
    
    do {
        if (m < temp_idx) {
            if (final_idx >= sizeof(final) - 1) {
                fprintf(stderr, "Final buffer overflow\n");
                return 1;
            }
            final[final_idx++] = temp[m];
        }
        m++;
    } while (m <= temp_idx);
    
    final[final_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", final);
    
    return 0;
}