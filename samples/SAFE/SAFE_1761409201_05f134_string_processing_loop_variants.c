//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LENGTH 256

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH * 2];
    char temp[MAX_LENGTH];
    
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
    
    if (len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    processed[0] = '\0';
    size_t proc_idx = 0;
    
    size_t i = 0;
    while (i < len) {
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0) {
            if (proc_idx + word_len * 2 + 2 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            size_t j;
            for (j = 0; j < word_len; j++) {
                temp[j] = input[word_start + j];
            }
            temp[word_len] = '\0';
            
            size_t k = 0;
            do {
                processed[proc_idx++] = temp[k];
                k++;
            } while (k < word_len);
            
            processed[proc_idx++] = ' ';
            
            size_t m = 0;
            while (m < word_len) {
                processed[proc_idx++] = temp[m];
                m++;
            }
            processed[proc_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0) {
        processed[proc_idx - 1] = '\0';
    } else {
        processed[0] = '\0';
    }
    
    printf("Processed string: %s\n", processed);
    
    char reversed[MAX_LENGTH * 2];
    size_t rev_idx = 0;
    size_t total_len = strlen(processed);
    
    if (total_len >= sizeof(reversed)) {
        fprintf(stderr, "String too long for reversal\n");
        return 1;
    }
    
    for (size_t idx = total_len; idx > 0; idx--) {
        reversed[rev_idx++] = processed[idx - 1];
    }
    reversed[rev_idx] = '\0';
    
    printf("Reversed result: %s\n", reversed);
    
    int digit_count = 0;
    size_t n = 0;
    for (n = 0; n < total_len; n++) {
        if (isdigit((unsigned char)processed[n])) {
            digit_count++;
        }
    }
    
    printf("Digit count in processed string: %d\n", digit_count);
    
    return 0;
}