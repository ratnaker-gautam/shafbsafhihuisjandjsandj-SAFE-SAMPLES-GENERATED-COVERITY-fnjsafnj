//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        for (; k < len && input[k] == current; k++) {
            count++;
        }
        
        if (count > 9) {
            fprintf(stderr, "Run length too long\n");
            return 1;
        }
        
        if (j + 2 >= sizeof(output)) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        output[j++] = current;
        output[j++] = '0' + count;
        
        i = k;
    }
    output[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Compressed: %s\n", output);
    
    char decompressed[MAX_LEN];
    int src_idx = 0;
    int dst_idx = 0;
    
    while (src_idx < j) {
        if (src_idx + 1 >= j) {
            fprintf(stderr, "Invalid compressed format\n");
            return 1;
        }
        
        char ch = output[src_idx++];
        int repeat = output[src_idx++] - '0';
        
        if (repeat < 1 || repeat > 9) {
            fprintf(stderr, "Invalid repeat count\n");
            return 1;
        }
        
        if (dst_idx + repeat >= sizeof(decompressed)) {
            fprintf(stderr, "Decompression buffer overflow\n");
            return 1;
        }
        
        int m;
        for (m = 0; m < repeat; m++) {
            decompressed[dst_idx++] = ch;
        }
    }
    decompressed[dst_idx] = '\0';
    
    printf("Decompressed: %s\n", decompressed);
    
    if (strcmp(input, decompressed) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}