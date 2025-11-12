//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char processed[MAX_INPUT * 2];
    char temp[MAX_INPUT];
    
    printf("Enter text to process: ");
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
        fprintf(stderr, "Empty input\n");
        return 1;
    }
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t p_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        size_t word_start = i;
        while (i < len && !isspace((unsigned char)input[i])) {
            i++;
        }
        
        size_t word_len = i - word_start;
        if (word_len > 0) {
            if (p_idx + word_len * 2 + 2 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (size_t j = 0; j < word_len; j++) {
                processed[p_idx++] = input[word_start + j];
            }
            
            processed[p_idx++] = ' ';
            
            for (size_t j = word_len; j > 0; j--) {
                processed[p_idx++] = input[word_start + j - 1];
            }
            
            processed[p_idx++] = ' ';
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    if (p_idx > 0) {
        processed[p_idx - 1] = '\0';
    } else {
        processed[0] = '\0';
    }
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t temp_idx = 0;
    for (size_t idx = 0; processed[idx] != '\0'; idx++) {
        if (temp_idx >= sizeof(temp) - 1) {
            fprintf(stderr, "Temp buffer overflow\n");
            return 1;
        }
        temp[temp_idx++] = processed[idx];
    }
    temp[temp_idx] = '\0';
    
    size_t count = 0;
    char *ptr = temp;
    while (*ptr) {
        if (*ptr == ' ') {
            count++;
        }
        ptr++;
    }
    
    printf("Word pairs: %zu\n", count / 2);
    
    return 0;
}