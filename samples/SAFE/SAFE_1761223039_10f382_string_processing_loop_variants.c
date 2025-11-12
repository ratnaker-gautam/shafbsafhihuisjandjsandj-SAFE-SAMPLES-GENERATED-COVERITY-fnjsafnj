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
        printf("Empty input string\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    processed[0] = '\0';
    size_t proc_idx = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (proc_idx >= sizeof(processed) - 3) {
            fprintf(stderr, "Output buffer overflow\n");
            return 1;
        }
        
        if (isalpha(input[i])) {
            processed[proc_idx++] = toupper(input[i]);
            processed[proc_idx++] = tolower(input[i]);
        } else if (isdigit(input[i])) {
            int digit = input[i] - '0';
            if (digit >= 0 && digit <= 9) {
                processed[proc_idx++] = input[i];
                processed[proc_idx++] = '0' + ((digit + 1) % 10);
            }
        } else {
            processed[proc_idx++] = input[i];
        }
    }
    processed[proc_idx] = '\0';
    
    size_t temp_idx = 0;
    size_t j = 0;
    while (j < proc_idx) {
        if (temp_idx >= sizeof(temp) - 1) {
            fprintf(stderr, "Temp buffer overflow\n");
            return 1;
        }
        temp[temp_idx++] = processed[j++];
    }
    temp[temp_idx] = '\0';
    
    char reversed[MAX_LEN * 2];
    size_t rev_idx = 0;
    size_t k = proc_idx;
    
    do {
        if (k == 0) break;
        k--;
        if (rev_idx >= sizeof(reversed) - 1) {
            fprintf(stderr, "Reversed buffer overflow\n");
            return 1;
        }
        reversed[rev_idx++] = processed[k];
    } while (k > 0);
    reversed[rev_idx] = '\0';
    
    size_t word_count = 0;
    int in_word = 0;
    
    for (size_t m = 0; m < proc_idx; m++) {
        if (isalnum(processed[m])) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Reversed: %s\n", reversed);
    printf("Word count: %zu\n", word_count);
    
    return 0;
}