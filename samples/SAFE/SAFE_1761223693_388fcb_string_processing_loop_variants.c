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
        printf("Empty input string\n");
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
            int written = snprintf(processed + p_idx, sizeof(processed) - p_idx, 
                                 "%c%zu", current, count);
            if (written < 0 || (size_t)written >= sizeof(processed) - p_idx) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            p_idx += written;
        } else {
            if (p_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[p_idx++] = current;
        }
        
        i = j;
    }
    
    if (p_idx >= sizeof(processed)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    processed[p_idx] = '\0';
    
    printf("Compressed: %s\n", processed);
    
    size_t decomp_idx = 0;
    size_t k = 0;
    while (k < p_idx) {
        char current = processed[k];
        
        if (k + 1 < p_idx && isdigit(processed[k + 1])) {
            size_t num_start = k + 1;
            size_t num_end = num_start;
            
            while (num_end < p_idx && isdigit(processed[num_end])) {
                num_end++;
            }
            
            if (num_end - num_start > 10) {
                fprintf(stderr, "Number too large\n");
                return 1;
            }
            
            strncpy(temp, processed + num_start, num_end - num_start);
            temp[num_end - num_start] = '\0';
            
            long count = strtol(temp, NULL, 10);
            if (count <= 0 || count > MAX_LENGTH) {
                fprintf(stderr, "Invalid count\n");
                return 1;
            }
            
            for (long m = 0; m < count; m++) {
                if (decomp_idx >= sizeof(temp) - 1) {
                    fprintf(stderr, "Decompression buffer overflow\n");
                    return 1;
                }
                temp[decomp_idx++] = current;
            }
            
            k = num_end;
        } else {
            if (decomp_idx >= sizeof(temp) - 1) {
                fprintf(stderr, "Decompression buffer overflow\n");
                return 1;
            }
            temp[decomp_idx++] = current;
            k++;
        }
    }
    
    if (decomp_idx >= sizeof(temp)) {
        fprintf(stderr, "Decompression buffer overflow\n");
        return 1;
    }
    temp[decomp_idx] = '\0';
    
    printf("Decompressed: %s\n", temp);
    
    if (strcmp(input, temp) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    return 0;
}