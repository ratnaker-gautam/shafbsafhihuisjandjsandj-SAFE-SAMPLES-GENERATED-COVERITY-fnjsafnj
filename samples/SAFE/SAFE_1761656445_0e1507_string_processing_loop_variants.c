//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

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
            if (proc_idx < sizeof(processed) - 2) {
                processed[proc_idx++] = toupper((unsigned char)current);
                processed[proc_idx++] = tolower((unsigned char)current);
            }
        } else if (isdigit((unsigned char)current)) {
            int digit = current - '0';
            for (int j = 0; j < digit && proc_idx < sizeof(processed) - 1; j++) {
                processed[proc_idx++] = '*';
            }
        } else {
            if (proc_idx < sizeof(processed) - 1) {
                processed[proc_idx++] = current;
            }
        }
        
        if (proc_idx >= sizeof(processed) - 1) {
            break;
        }
        
        i++;
    }
    
    processed[proc_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t char_count[256] = {0};
    for (size_t idx = 0; idx < proc_idx; idx++) {
        unsigned char c = processed[idx];
        if (char_count[c] < SIZE_MAX) {
            char_count[c]++;
        }
    }
    
    printf("Character frequencies:\n");
    for (int ch = 0; ch < 256; ch++) {
        if (char_count[ch] > 0 && isprint(ch)) {
            printf("'%c': %zu\n", ch, char_count[ch]);
        }
    }
    
    char reversed[MAX_INPUT_LEN + 1];
    size_t rev_idx = 0;
    size_t pos = proc_idx;
    
    do {
        if (pos > 0) {
            pos--;
            if (rev_idx < sizeof(reversed) - 1) {
                reversed[rev_idx++] = processed[pos];
            }
        }
    } while (pos > 0 && rev_idx < sizeof(reversed) - 1);
    
    reversed[rev_idx] = '\0';
    printf("Reversed: %s\n", reversed);
    
    return 0;
}