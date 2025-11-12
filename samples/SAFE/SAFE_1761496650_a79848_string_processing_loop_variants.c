//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    
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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t proc_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        char current = input[i];
        
        if (isalpha(current)) {
            if (proc_idx >= sizeof(processed) - 3) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[proc_idx++] = toupper(current);
            processed[proc_idx++] = '-';
        } else if (isdigit(current)) {
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            int digit = current - '0';
            for (int j = 0; j < digit && proc_idx < sizeof(processed) - 1; j++) {
                processed[proc_idx++] = '*';
            }
        } else {
            if (proc_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = current;
        }
        
        i++;
    }
    
    for (size_t j = 0; j < len; j++) {
        size_t k = j;
        int count = 1;
        while (k + 1 < len && input[k] == input[k + 1]) {
            count++;
            k++;
        }
        
        if (count > 1) {
            if (proc_idx >= sizeof(processed) - 10) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            int written = snprintf(processed + proc_idx, 
                                 sizeof(processed) - proc_idx,
                                 "[%c:%d]", input[j], count);
            if (written < 0 || (size_t)written >= sizeof(processed) - proc_idx) {
                fprintf(stderr, "Formatting error\n");
                return 1;
            }
            proc_idx += written;
            j += count - 1;
        }
    }
    
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t char_count = 0;
    size_t digit_count = 0;
    size_t other_count = 0;
    
    for (char *ptr = input; *ptr != '\0'; ptr++) {
        if (isalpha(*ptr)) {
            char_count++;
        } else if (isdigit(*ptr)) {
            digit_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Statistics: %zu letters, %zu digits, %zu other characters\n",
           char_count, digit_count, other_count);
    
    return 0;
}