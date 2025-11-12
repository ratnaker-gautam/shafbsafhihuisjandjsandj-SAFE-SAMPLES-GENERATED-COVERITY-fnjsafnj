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
        
        if (isalpha((unsigned char)current)) {
            if (proc_idx >= sizeof(processed) - 3) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[proc_idx++] = toupper((unsigned char)current);
            processed[proc_idx++] = '-';
        }
        
        i++;
    }
    
    for (size_t j = 0; j < input_len; j++) {
        char current = input[j];
        
        if (isdigit((unsigned char)current)) {
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[proc_idx++] = current;
            processed[proc_idx++] = ' ';
        }
    }
    
    size_t k = 0;
    do {
        if (k >= input_len) break;
        
        char current = input[k];
        if (!isalnum((unsigned char)current) && !isspace((unsigned char)current)) {
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[proc_idx++] = '*';
            processed[proc_idx++] = '*';
        }
        
        k++;
    } while (k < input_len);
    
    if (proc_idx > 0) {
        if (processed[proc_idx - 1] == '-' || processed[proc_idx - 1] == ' ') {
            proc_idx--;
        }
    }
    
    if (proc_idx >= sizeof(processed)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t char_count = 0;
    size_t digit_count = 0;
    size_t other_count = 0;
    
    for (size_t idx = 0; idx < input_len; idx++) {
        char ch = input[idx];
        if (isalpha((unsigned char)ch)) {
            char_count++;
        } else if (isdigit((unsigned char)ch)) {
            digit_count++;
        } else if (!isspace((unsigned char)ch)) {
            other_count++;
        }
    }
    
    printf("Statistics: %zu letters, %zu digits, %zu other characters\n", 
           char_count, digit_count, other_count);
    
    return 0;
}