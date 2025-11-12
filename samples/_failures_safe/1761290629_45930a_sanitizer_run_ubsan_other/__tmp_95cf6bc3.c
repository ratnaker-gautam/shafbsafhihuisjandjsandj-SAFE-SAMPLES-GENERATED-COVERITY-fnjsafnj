//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    char temp[MAX_LENGTH];
    
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
    
    size_t p_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        char current = input[i];
        size_t count = 1;
        
        size_t j = i + 1;
        while (j < len && input[j] == current) {
            count++;
            j++;
        }
        
        if (count > 1) {
            int written = snprintf(&processed[p_idx], sizeof(processed) - p_idx, 
                                 "%c%zu", current, count);
            if (written < 0 || (size_t)written >= sizeof(processed) - p_idx) {
                fprintf(stderr, "Buffer overflow prevented\n");
                return 1;
            }
            p_idx += written;
        } else {
            if (p_idx < sizeof(processed) - 1) {
                processed[p_idx++] = current;
            } else {
                fprintf(stderr, "Buffer overflow prevented\n");
                return 1;
            }
        }
        
        i = j;
    }
    
    if (p_idx < sizeof(processed)) {
        processed[p_idx] = '\0';
    } else {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    printf("Compressed: %s\n", processed);
    
    size_t temp_idx = 0;
    size_t k = 0;
    
    for (k = 0; k < p_idx && temp_idx < sizeof(temp) - 1; k++) {
        if (isdigit((unsigned char)processed[k])) {
            char character = processed[k - 1];
            size_t num = 0;
            
            size_t m = k;
            while (m < p_idx && isdigit((unsigned char)processed[m])) {
                num = num * 10 + (processed[m] - '0');
                m++;
                
                if (num > sizeof(temp) - temp_idx) {
                    fprintf(stderr, "Expansion too large\n");
                    return 1;
                }
            }
            
            size_t n;
            for (n = 0; n < num && temp_idx < sizeof(temp) - 1; n++) {
                temp[temp_idx++] = character;
            }
            
            k = m - 1;
        } else {
            temp[temp_idx++] = processed[k];
        }
    }
    
    if (temp_idx < sizeof(temp)) {
        temp[temp_idx] = '\0';
    } else {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    printf("Decompressed: %s\n", temp);
    
    if (strcmp(input, temp) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip mismatch\n");
    }
    
    return 0;
}