//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
                if (proc_idx >= sizeof(processed) - 2) {
                    fprintf(stderr, "Output buffer overflow\n");
                    return 1;
                }
                processed[proc_idx++] = temp[j];
            } while (++j < word_len);
            
            if (proc_idx >= sizeof(processed) - 2) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            processed[proc_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (proc_idx > 0 && processed[proc_idx - 1] == ' ') {
        proc_idx--;
    }
    processed[proc_idx] = '\0';
    
    printf("Processed string: %s\n", processed);
    
    size_t char_count = 0;
    for (size_t k = 0; k < proc_idx; k++) {
        if (processed[k] != ' ') {
            char_count++;
        }
    }
    printf("Non-space characters: %zu\n", char_count);
    
    size_t word_count = 0;
    size_t pos = 0;
    while (pos < proc_idx) {
        while (pos < proc_idx && processed[pos] == ' ') {
            pos++;
        }
        if (pos < proc_idx) {
            word_count++;
            while (pos < proc_idx && processed[pos] != ' ') {
                pos++;
            }
        }
    }
    printf("Word count: %zu\n", word_count);
    
    return 0;
}