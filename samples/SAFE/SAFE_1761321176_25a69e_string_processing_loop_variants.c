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
            if (proc_idx >= sizeof(processed) - 3) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            processed[proc_idx++] = toupper(current);
            processed[proc_idx++] = '-';
        }
        
        i++;
    }
    
    if (proc_idx > 0) {
        processed[proc_idx - 1] = '\0';
    } else {
        processed[0] = '\0';
    }
    
    size_t word_count = 0;
    for (size_t j = 0; j < proc_idx; j++) {
        if (processed[j] == '-') {
            word_count++;
        }
    }
    
    if (proc_idx > 0) {
        word_count++;
    }
    
    char reversed[MAX_INPUT_LEN * 2 + 1];
    size_t rev_idx = 0;
    size_t k = proc_idx;
    
    do {
        if (k > 0) {
            k--;
            if (rev_idx >= sizeof(reversed) - 1) {
                fprintf(stderr, "Reversal buffer overflow\n");
                return 1;
            }
            reversed[rev_idx++] = processed[k];
        }
    } while (k > 0);
    
    reversed[rev_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Word count: %zu\n", word_count);
    printf("Reversed: %s\n", reversed);
    
    return 0;
}