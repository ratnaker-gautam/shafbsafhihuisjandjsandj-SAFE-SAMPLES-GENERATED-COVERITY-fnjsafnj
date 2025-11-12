//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    
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
            if (proc_idx < MAX_INPUT_LEN) {
                processed[proc_idx] = toupper((unsigned char)current);
                proc_idx++;
            }
        } else if (isdigit((unsigned char)current)) {
            if (proc_idx < MAX_INPUT_LEN) {
                processed[proc_idx] = current;
                proc_idx++;
            }
        } else if (isspace((unsigned char)current)) {
            if (proc_idx < MAX_INPUT_LEN) {
                processed[proc_idx] = ' ';
                proc_idx++;
            }
        }
        i++;
    }
    
    if (proc_idx == 0) {
        fprintf(stderr, "No valid characters found\n");
        return 1;
    }
    
    processed[proc_idx] = '\0';
    
    size_t final_len = strlen(processed);
    char compressed[MAX_INPUT_LEN + 1];
    size_t comp_idx = 0;
    
    for (size_t j = 0; j < final_len; j++) {
        if (comp_idx >= MAX_INPUT_LEN) {
            break;
        }
        
        compressed[comp_idx] = processed[j];
        comp_idx++;
        
        if (j < final_len - 1 && processed[j] == processed[j + 1]) {
            size_t count = 1;
            size_t k = j + 1;
            
            do {
                count++;
                k++;
            } while (k < final_len && processed[k] == processed[j] && count < 9);
            
            if (comp_idx < MAX_INPUT_LEN) {
                compressed[comp_idx] = '0' + count;
                comp_idx++;
            }
            
            j = k - 1;
        }
    }
    
    compressed[comp_idx] = '\0';
    
    char reversed[MAX_INPUT_LEN + 1];
    size_t rev_idx = 0;
    
    for (size_t m = comp_idx; m > 0; m--) {
        if (rev_idx < MAX_INPUT_LEN) {
            reversed[rev_idx] = compressed[m - 1];
            rev_idx++;
        }
    }
    reversed[rev_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Compressed: %s\n", compressed);
    printf("Reversed: %s\n", reversed);
    
    return 0;
}