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
    
    processed[0] = '\0';
    size_t proc_idx = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (proc_idx >= sizeof(processed) - 10) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (isalpha(input[i])) {
            char current = input[i];
            int count = 1;
            
            size_t j = i + 1;
            while (j < len && input[j] == current) {
                count++;
                j++;
            }
            
            if (count > 1) {
                int written = snprintf(processed + proc_idx, 
                                     sizeof(processed) - proc_idx,
                                     "%c%d", current, count);
                if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                    fprintf(stderr, "Buffer overflow\n");
                    return 1;
                }
                proc_idx += written;
            } else {
                processed[proc_idx++] = current;
                processed[proc_idx] = '\0';
            }
            
            i = j - 1;
        } else if (isdigit(input[i])) {
            int sum = 0;
            size_t k = i;
            while (k < len && isdigit(input[k])) {
                sum += (input[k] - '0');
                k++;
            }
            
            int written = snprintf(processed + proc_idx,
                                 sizeof(processed) - proc_idx,
                                 "[%d]", sum);
            if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                fprintf(stderr, "Buffer overflow\n");
                return 1;
            }
            proc_idx += written;
            i = k - 1;
        } else {
            processed[proc_idx++] = input[i];
            processed[proc_idx] = '\0';
        }
    }
    
    size_t final_len = strlen(processed);
    if (final_len >= sizeof(temp)) {
        fprintf(stderr, "Intermediate buffer overflow\n");
        return 1;
    }
    
    strcpy(temp, processed);
    
    size_t rev_idx = 0;
    for (size_t m = final_len; m > 0; m--) {
        if (rev_idx >= sizeof(processed) - 1) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        processed[rev_idx++] = temp[m - 1];
    }
    processed[rev_idx] = '\0';
    
    char filtered[MAX_LEN];
    size_t filter_idx = 0;
    
    size_t n = 0;
    while (n < strlen(processed)) {
        if (filter_idx >= sizeof(filtered) - 1) {
            fprintf(stderr, "Filter buffer overflow\n");
            return 1;
        }
        
        if (!ispunct(processed[n])) {
            filtered[filter_idx++] = processed[n];
        }
        n++;
    }
    filtered[filter_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", filtered);
    
    return 0;
}