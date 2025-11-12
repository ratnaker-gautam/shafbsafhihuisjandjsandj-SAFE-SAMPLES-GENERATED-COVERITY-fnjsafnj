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
    
    printf("Enter a string (max %d chars): ", MAX_LENGTH - 1);
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
    
    if (len >= MAX_LENGTH) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    j = 0;
    i = 0;
    while (i < len) {
        if (isalpha((unsigned char)input[i])) {
            char current = input[i];
            int count = 1;
            
            k = i + 1;
            while (k < len && input[k] == current) {
                count++;
                k++;
            }
            
            if (count > 1) {
                int written = snprintf(processed + j, sizeof(processed) - j, "%c%d", current, count);
                if (written < 0 || written >= (int)(sizeof(processed) - j)) {
                    fprintf(stderr, "Buffer overflow prevented\n");
                    return 1;
                }
                j += written;
            } else {
                if (j < (int)sizeof(processed) - 1) {
                    processed[j++] = current;
                } else {
                    fprintf(stderr, "Buffer overflow prevented\n");
                    return 1;
                }
            }
            
            i = k;
        } else {
            if (j < (int)sizeof(processed) - 1) {
                processed[j++] = input[i++];
            } else {
                fprintf(stderr, "Buffer overflow prevented\n");
                return 1;
            }
        }
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Compressed: %s\n", processed);
    
    char decompressed[MAX_LENGTH];
    int src_idx = 0;
    int dst_idx = 0;
    
    while (src_idx < j && dst_idx < MAX_LENGTH - 1) {
        if (isalpha((unsigned char)processed[src_idx])) {
            char ch = processed[src_idx++];
            
            if (src_idx < j && isdigit((unsigned char)processed[src_idx])) {
                int num = 0;
                while (src_idx < j && isdigit((unsigned char)processed[src_idx])) {
                    num = num * 10 + (processed[src_idx] - '0');
                    src_idx++;
                    
                    if (dst_idx + num >= MAX_LENGTH - 1) {
                        fprintf(stderr, "Decompression would overflow buffer\n");
                        return 1;
                    }
                }
                
                for (int rep = 0; rep < num && dst_idx < MAX_LENGTH - 1; rep++) {
                    decompressed[dst_idx++] = ch;
                }
            } else {
                decompressed[dst_idx++] = ch;
            }
        } else {
            decompressed[dst_idx++] = processed[src_idx++];
        }
    }
    
    decompressed[dst_idx] = '\0';
    
    printf("Decompressed: %s\n", decompressed);
    
    if (strcmp(input, decompressed) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    return 0;
}