//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
    
    processed[0] = '\0';
    size_t proc_idx = 0;
    
    int i = 0;
    while (input[i] != '\0') {
        if (isalpha((unsigned char)input[i])) {
            int count = 1;
            char current = input[i];
            
            int j = i + 1;
            for (; input[j] != '\0' && input[j] == current; j++) {
                count++;
            }
            
            if (count > 1) {
                int written = snprintf(temp, sizeof(temp), "%c%d", current, count);
                if (written < 0 || (size_t)written >= sizeof(temp)) {
                    fprintf(stderr, "Buffer overflow prevented\n");
                    return 1;
                }
            } else {
                temp[0] = current;
                temp[1] = '\0';
            }
            
            size_t temp_len = strlen(temp);
            if (proc_idx + temp_len >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            strcpy(processed + proc_idx, temp);
            proc_idx += temp_len;
            i += (count > 1) ? count : 1;
        } else if (isdigit((unsigned char)input[i])) {
            int sum = 0;
            int k = i;
            do {
                if (isdigit((unsigned char)input[k])) {
                    sum += (input[k] - '0');
                }
                k++;
            } while (isdigit((unsigned char)input[k]));
            
            int written = snprintf(temp, sizeof(temp), "[%d]", sum);
            if (written < 0 || (size_t)written >= sizeof(temp)) {
                fprintf(stderr, "Buffer overflow prevented\n");
                return 1;
            }
            
            size_t temp_len = strlen(temp);
            if (proc_idx + temp_len >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            strcpy(processed + proc_idx, temp);
            proc_idx += temp_len;
            i = k;
        } else {
            if (proc_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = input[i++];
            processed[proc_idx] = '\0';
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LEN];
    size_t rev_idx = 0;
    int pos = len - 1;
    for (; pos >= 0; pos--) {
        if (rev_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversal buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = input[pos];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    return 0;
}