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
    
    for (size_t j = 0; j < input_len; j++) {
        if (input[j] == ' ') {
            if (proc_idx >= sizeof(processed) - 6) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            memcpy(&processed[proc_idx], "[SPACE]", 7);
            proc_idx += 7;
        }
    }
    
    processed[proc_idx] = '\0';
    
    size_t final_len = strlen(processed);
    if (final_len > 0) {
        printf("Processed string (%zu characters):\n", final_len);
        
        size_t pos = 0;
        do {
            if (pos < final_len) {
                putchar(processed[pos]);
            }
            pos++;
        } while (pos <= final_len);
        putchar('\n');
        
        printf("Character counts:\n");
        int counts[256] = {0};
        for (size_t k = 0; k < final_len; k++) {
            unsigned char c = processed[k];
            if (c < 256) {
                counts[c]++;
            }
        }
        
        for (int c = 0; c < 256; c++) {
            if (counts[c] > 0 && isprint(c)) {
                printf("'%c': %d\n", c, counts[c]);
            }
        }
    }
    
    return 0;
}