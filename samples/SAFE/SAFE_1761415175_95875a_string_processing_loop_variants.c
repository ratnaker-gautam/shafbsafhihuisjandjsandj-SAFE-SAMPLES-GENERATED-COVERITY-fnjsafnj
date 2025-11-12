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
            
            if (count > 9) {
                fprintf(stderr, "Run length too long\n");
                return 1;
            }
            
            int written = snprintf(processed + proc_idx, 
                                 sizeof(processed) - proc_idx,
                                 "%c%d", current, count);
            if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                fprintf(stderr, "Formatting error\n");
                return 1;
            }
            proc_idx += written;
            i = j - 1;
        } else if (isdigit(input[i])) {
            char digit = input[i];
            int value = digit - '0';
            
            if (value < 1 || value > 9) {
                fprintf(stderr, "Invalid digit\n");
                return 1;
            }
            
            if (proc_idx + value >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (int k = 0; k < value; k++) {
                processed[proc_idx++] = '*';
            }
            processed[proc_idx] = '\0';
        } else {
            if (proc_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = input[i];
            processed[proc_idx] = '\0';
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    strcpy(temp, processed);
    size_t temp_len = strlen(temp);
    
    char reversed[MAX_LEN * 2];
    reversed[0] = '\0';
    size_t rev_idx = 0;
    
    if (temp_len > 0) {
        size_t k = temp_len;
        do {
            k--;
            if (rev_idx >= sizeof(reversed) - 1) {
                fprintf(stderr, "Reversal buffer overflow\n");
                return 1;
            }
            reversed[rev_idx++] = temp[k];
        } while (k > 0);
        reversed[rev_idx] = '\0';
    }
    
    printf("Reversed: %s\n", reversed);
    
    int word_count = 0;
    size_t m = 0;
    while (m < proc_idx) {
        while (m < proc_idx && isspace(processed[m])) {
            m++;
        }
        
        if (m < proc_idx) {
            word_count++;
            while (m < proc_idx && !isspace(processed[m])) {
                m++;
            }
        }
    }
    
    printf("Word count: %d\n", word_count);
    
    return 0;
}