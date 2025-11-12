//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 256

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN * 2 + 1];
    
    printf("Enter a string to process: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t proc_idx = 0;
    size_t i = 0;
    
    while (i < input_len) {
        char current = input[i];
        
        if (isalpha(current)) {
            processed[proc_idx++] = toupper(current);
            
            int count = 1;
            size_t j = i + 1;
            for (; j < input_len && input[j] == current; j++) {
                count++;
            }
            
            if (count > 1) {
                char num_str[12];
                int written = snprintf(num_str, sizeof(num_str), "(%d)", count);
                if (written < 0 || (size_t)written >= sizeof(num_str)) {
                    fprintf(stderr, "Number formatting error\n");
                    return 1;
                }
                
                size_t k = 0;
                do {
                    if (proc_idx >= sizeof(processed) - 1) {
                        fprintf(stderr, "Output buffer overflow\n");
                        return 1;
                    }
                    processed[proc_idx++] = num_str[k];
                    k++;
                } while (k < (size_t)written && num_str[k] != '\0');
            }
            
            i = j;
        } else if (isdigit(current)) {
            int digit_val = current - '0';
            char repeat_char = 'X';
            
            for (int rep = 0; rep < digit_val; rep++) {
                if (proc_idx >= sizeof(processed) - 1) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = repeat_char;
            }
            
            i++;
        } else {
            if (proc_idx >= sizeof(processed) - 1) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = current;
            i++;
        }
    }
    
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t char_count = 0;
    size_t digit_count = 0;
    size_t other_count = 0;
    
    for (size_t idx = 0; idx < input_len; idx++) {
        char ch = input[idx];
        if (isalpha(ch)) {
            char_count++;
        } else if (isdigit(ch)) {
            digit_count++;
        } else {
            other_count++;
        }
    }
    
    printf("Statistics:\n");
    printf("  Letters: %zu\n", char_count);
    printf("  Digits: %zu\n", digit_count);
    printf("  Other: %zu\n", other_count);
    
    return 0;
}