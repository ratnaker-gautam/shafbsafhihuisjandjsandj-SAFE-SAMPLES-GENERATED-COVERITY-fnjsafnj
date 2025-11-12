//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
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
        if (isalpha((unsigned char)input[i])) {
            char current = input[i];
            int count = 1;
            
            k = i + 1;
            while (k < len && input[k] == current) {
                count++;
                k++;
            }
            
            if (j + 3 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[j++] = current;
            if (count > 1) {
                char num_str[16];
                int written = snprintf(num_str, sizeof(num_str), "%d", count);
                if (written < 0 || (size_t)written >= sizeof(num_str)) {
                    fprintf(stderr, "Number conversion error\n");
                    return 1;
                }
                
                for (k = 0; k < written; k++) {
                    if (j >= sizeof(processed) - 1) {
                        fprintf(stderr, "Output buffer overflow\n");
                        return 1;
                    }
                    processed[j++] = num_str[k];
                }
            }
            
            i += count;
        } else {
            if (j >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[j++] = input[i++];
        }
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Compressed: %s\n", processed);
    
    char decompressed[MAX_LEN];
    i = 0;
    j = 0;
    while (i < strlen(processed)) {
        if (j >= sizeof(decompressed) - 1) {
            fprintf(stderr, "Decompression buffer overflow\n");
            return 1;
        }
        
        if (isalpha((unsigned char)processed[i])) {
            char current = processed[i];
            int count = 1;
            
            if (i + 1 < strlen(processed) && isdigit((unsigned char)processed[i + 1])) {
                count = 0;
                k = i + 1;
                while (k < strlen(processed) && isdigit((unsigned char)processed[k])) {
                    if (count > (MAX_LEN / 10)) {
                        fprintf(stderr, "Count too large\n");
                        return 1;
                    }
                    count = count * 10 + (processed[k] - '0');
                    k++;
                }
                
                if (count <= 0 || j + count >= sizeof(decompressed)) {
                    fprintf(stderr, "Invalid count or buffer overflow\n");
                    return 1;
                }
                
                for (int m = 0; m < count; m++) {
                    decompressed[j++] = current;
                }
                
                i = k;
            } else {
                decompressed[j++] = current;
                i++;
            }
        } else {
            decompressed[j++] = processed[i++];
        }
    }
    
    decompressed[j] = '\0';
    
    printf("Decompressed: %s\n", decompressed);
    
    if (strcmp(input, decompressed) == 0) {
        printf("Round-trip successful\n");
    } else {
        printf("Round-trip failed\n");
    }
    
    return 0;
}