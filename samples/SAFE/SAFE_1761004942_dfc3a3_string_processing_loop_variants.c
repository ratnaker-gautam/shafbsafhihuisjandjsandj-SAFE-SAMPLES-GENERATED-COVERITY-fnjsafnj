//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, count;
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
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
    count = 1;
    
    for (i = 0; i < len; i++) {
        if (i + 1 < len && input[i] == input[i + 1]) {
            count++;
        } else {
            if (j + 3 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            int written = snprintf(processed + j, sizeof(processed) - j, "%c%d", input[i], count);
            if (written < 0 || written >= (int)(sizeof(processed) - j)) {
                fprintf(stderr, "Encoding error\n");
                return 1;
            }
            j += written;
            count = 1;
        }
    }
    
    processed[j] = '\0';
    
    printf("Original: %s\n", input);
    printf("Compressed: %s\n", processed);
    
    char decompressed[MAX_LENGTH];
    int k = 0;
    i = 0;
    
    while (i < j) {
        if (k >= MAX_LENGTH - 1) {
            fprintf(stderr, "Decompression buffer overflow\n");
            return 1;
        }
        
        char ch = processed[i++];
        if (i >= j || !isdigit((unsigned char)processed[i])) {
            fprintf(stderr, "Invalid compressed format\n");
            return 1;
        }
        
        int num = 0;
        while (i < j && isdigit((unsigned char)processed[i])) {
            num = num * 10 + (processed[i] - '0');
            i++;
            if (num > MAX_LENGTH) {
                fprintf(stderr, "Decompression count too large\n");
                return 1;
            }
        }
        
        if (k + num >= MAX_LENGTH) {
            fprintf(stderr, "Decompression would overflow buffer\n");
            return 1;
        }
        
        int m;
        for (m = 0; m < num; m++) {
            decompressed[k++] = ch;
        }
    }
    
    decompressed[k] = '\0';
    
    printf("Decompressed: %s\n", decompressed);
    
    if (strcmp(input, decompressed) == 0) {
        printf("Verification: SUCCESS\n");
    } else {
        printf("Verification: FAILED\n");
        return 1;
    }
    
    return 0;
}