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
                    fprintf(stderr, "Formatting error\n");
                    return 1;
                }
                proc_idx += written;
            } else {
                processed[proc_idx++] = current;
                processed[proc_idx] = '\0';
            }
            
            i = j - 1;
        } else if (isdigit(input[i])) {
            size_t start = i;
            while (i < len && isdigit(input[i])) {
                i++;
            }
            
            size_t num_len = i - start;
            if (num_len > 0) {
                strncpy(temp, input + start, num_len);
                temp[num_len] = '\0';
                
                char *endptr;
                long num = strtol(temp, &endptr, 10);
                if (endptr == temp || *endptr != '\0' || num < 0) {
                    fprintf(stderr, "Invalid number format\n");
                    return 1;
                }
                
                int written = snprintf(processed + proc_idx,
                                     sizeof(processed) - proc_idx,
                                     "[%ld]", num);
                if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                    fprintf(stderr, "Formatting error\n");
                    return 1;
                }
                proc_idx += written;
            }
            i--;
        } else {
            processed[proc_idx++] = input[i];
            processed[proc_idx] = '\0';
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    char reversed[MAX_LEN];
    size_t rev_idx = 0;
    size_t k = strlen(processed);
    
    while (k > 0) {
        k--;
        if (rev_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversal buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = processed[k];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed: %s\n", reversed);
    
    int alpha_count = 0;
    int digit_count = 0;
    int other_count = 0;
    
    for (size_t m = 0; m < strlen(input); m++) {
        if (isalpha(input[m])) {
            alpha_count++;
        } else if (isdigit(input[m])) {
            digit_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Statistics: %d alphabetic, %d digits, %d other\n", 
           alpha_count, digit_count, other_count);
    
    return 0;
}