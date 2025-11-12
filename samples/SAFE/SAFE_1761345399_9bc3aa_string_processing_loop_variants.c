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
    
    if (len >= MAX_LEN) {
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
            if (p_idx + word_len * 2 + 1 >= sizeof(processed)) {
                fprintf(stderr, "Output buffer overflow\n");
                return 1;
            }
            
            for (size_t j = 0; j < word_len; j++) {
                processed[p_idx++] = input[word_start + j];
                processed[p_idx++] = input[word_start + j];
            }
            
            if (i < len) {
                processed[p_idx++] = ' ';
            }
        }
        
        while (i < len && isspace((unsigned char)input[i])) {
            i++;
        }
    }
    
    processed[p_idx] = '\0';
    
    printf("Original: '%s'\n", input);
    printf("Processed: '%s'\n", processed);
    
    size_t temp_idx = 0;
    for (size_t k = 0; k < p_idx; k++) {
        if (temp_idx >= sizeof(temp) - 1) {
            break;
        }
        if (isalpha((unsigned char)processed[k])) {
            temp[temp_idx++] = toupper((unsigned char)processed[k]);
        }
    }
    temp[temp_idx] = '\0';
    
    printf("Uppercase letters: '%s'\n", temp);
    
    size_t count = 0;
    for (size_t m = 0; m < temp_idx; m++) {
        if (temp[m] == 'A' || temp[m] == 'E' || temp[m] == 'I' || 
            temp[m] == 'O' || temp[m] == 'U') {
            count++;
        }
    }
    
    printf("Vowel count in uppercase: %zu\n", count);
    
    return 0;
}