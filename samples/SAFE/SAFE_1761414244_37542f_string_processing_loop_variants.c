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
    
    size_t i = 0;
    while (i < len) {
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0) {
            if (word_len >= sizeof(temp)) {
                fprintf(stderr, "Word too long\n");
                return 1;
            }
            
            strncpy(temp, &input[word_start], word_len);
            temp[word_len] = '\0';
            
            size_t j = 0;
            do {
                if (j < word_len) {
                    temp[j] = toupper((unsigned char)temp[j]);
                    j++;
                }
            } while (j < word_len);
            
            for (size_t k = 0; k < word_len; k++) {
                if (proc_idx >= sizeof(processed) - 2) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = temp[k];
            }
            
            if (i < len && proc_idx < sizeof(processed) - 1) {
                processed[proc_idx++] = ' ';
            }
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    
    if (proc_idx >= sizeof(processed)) {
        fprintf(stderr, "Output buffer overflow\n");
        return 1;
    }
    processed[proc_idx] = '\0';
    
    char reversed[MAX_LEN * 2];
    size_t rev_idx = 0;
    size_t pos = proc_idx;
    
    while (pos > 0) {
        size_t word_end = pos;
        while (pos > 0 && processed[pos - 1] != ' ') {
            pos--;
        }
        
        size_t copy_len = word_end - pos;
        if (copy_len > 0) {
            if (rev_idx + copy_len >= sizeof(reversed) - 1) {
                fprintf(stderr, "Reversed buffer overflow\n");
                return 1;
            }
            
            for (size_t m = 0; m < copy_len; m++) {
                reversed[rev_idx++] = processed[pos + m];
            }
            
            if (pos > 0 && rev_idx < sizeof(reversed) - 1) {
                reversed[rev_idx++] = ' ';
            }
        }
        
        if (pos > 0) {
            pos--;
        }
    }
    
    if (rev_idx >= sizeof(reversed)) {
        fprintf(stderr, "Reversed buffer overflow\n");
        return 1;
    }
    reversed[rev_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Uppercase: %s\n", processed);
    printf("Reversed order: %s\n", reversed);
    
    return 0;
}