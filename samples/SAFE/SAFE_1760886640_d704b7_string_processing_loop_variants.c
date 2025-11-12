//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    int i, j, k;
    
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
    
    j = 0;
    for (i = 0; i < len; i++) {
        if (isalnum((unsigned char)input[i])) {
            processed[j++] = toupper((unsigned char)input[i]);
        }
    }
    
    if (j == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    processed[j] = '\0';
    
    char reversed[MAX_LENGTH * 2];
    k = 0;
    int pos = j - 1;
    while (pos >= 0) {
        reversed[k++] = processed[pos--];
    }
    reversed[k] = '\0';
    
    char interleaved[MAX_LENGTH * 4];
    int m = 0;
    int n = 0;
    int p = 0;
    
    while (processed[m] != '\0' || reversed[n] != '\0') {
        if (processed[m] != '\0') {
            interleaved[p++] = processed[m++];
        }
        if (reversed[n] != '\0') {
            interleaved[p++] = reversed[n++];
        }
    }
    interleaved[p] = '\0';
    
    if (p >= sizeof(interleaved) - 1) {
        fprintf(stderr, "Buffer overflow prevented\n");
        return 1;
    }
    
    char final[MAX_LENGTH * 4];
    int count = 1;
    int idx = 0;
    
    for (int q = 0; q < p; q++) {
        final[idx++] = interleaved[q];
        
        if ((q + 1) % 3 == 0 && (q + 1) < p) {
            final[idx++] = '-';
            count++;
        }
        
        if (idx >= sizeof(final) - 2) {
            break;
        }
    }
    final[idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Interleaved: %s\n", interleaved);
    printf("Final formatted: %s\n", final);
    
    return 0;
}