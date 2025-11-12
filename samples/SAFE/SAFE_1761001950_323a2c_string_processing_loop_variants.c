//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char output[MAX_LEN * 2];
    int i, j, k;
    
    printf("Enter a string (max %d chars): ", MAX_LEN - 1);
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
    
    j = 0;
    i = 0;
    while (i < len) {
        char current = input[i];
        int count = 1;
        
        k = i + 1;
        while (k < len && input[k] == current) {
            count++;
            k++;
        }
        
        if (j >= sizeof(output) - 10) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (count > 1) {
            int written = snprintf(output + j, sizeof(output) - j, "%c%d", current, count);
            if (written < 0 || written >= (int)(sizeof(output) - j)) {
                fprintf(stderr, "Formatting error\n");
                return 1;
            }
            j += written;
        } else {
            output[j++] = current;
        }
        
        i = k;
    }
    output[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Compressed: %s\n", output);
    
    char decompressed[MAX_LEN];
    int idx = 0;
    i = 0;
    size_t out_len = strlen(output);
    
    for (i = 0; i < out_len && idx < MAX_LEN - 1; ) {
        char ch = output[i];
        
        if (i + 1 < out_len && isdigit(output[i + 1])) {
            int num = 0;
            k = i + 1;
            while (k < out_len && isdigit(output[k])) {
                num = num * 10 + (output[k] - '0');
                k++;
            }
            
            if (num <= 0 || num > MAX_LEN) {
                fprintf(stderr, "Invalid compression format\n");
                return 1;
            }
            
            int m;
            for (m = 0; m < num && idx < MAX_LEN - 1; m++) {
                decompressed[idx++] = ch;
            }
            
            i = k;
        } else {
            decompressed[idx++] = ch;
            i++;
        }
    }
    decompressed[idx] = '\0';
    
    if (strcmp(input, decompressed) == 0) {
        printf("Decompression verified: %s\n", decompressed);
    } else {
        printf("Decompression mismatch\n");
        return 1;
    }
    
    return 0;
}