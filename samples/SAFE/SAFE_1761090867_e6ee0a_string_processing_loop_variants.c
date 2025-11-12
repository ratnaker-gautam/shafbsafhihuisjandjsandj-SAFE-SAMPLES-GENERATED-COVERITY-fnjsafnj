//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: loop_variants ; Variation: string_processing
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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LENGTH - 1) {
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
                processed[p_idx++] = input[i++];
            }
        } else {
            i++;
        }
    }
    
    processed[p_idx] = '\0';
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    
    size_t temp_idx = 0;
    for (size_t idx = 0; idx < p_idx; idx++) {
        if (temp_idx >= sizeof(temp) - 1) {
            break;
        }
        if (isalpha((unsigned char)processed[idx])) {
            temp[temp_idx++] = toupper((unsigned char)processed[idx]);
        }
    }
    temp[temp_idx] = '\0';
    
    if (temp_idx > 0) {
        printf("Uppercase letters: %s\n", temp);
        
        size_t count = 0;
        for (size_t idx = 0; idx < temp_idx; idx++) {
            if (temp[idx] == 'A' || temp[idx] == 'E' || temp[idx] == 'I' || 
                temp[idx] == 'O' || temp[idx] == 'U') {
                count++;
            }
        }
        printf("Vowel count: %zu\n", count);
    }
    
    char reversed[MAX_LENGTH];
    size_t r_idx = 0;
    if (len > 0) {
        for (size_t idx = len; idx > 0; idx--) {
            if (r_idx >= sizeof(reversed) - 1) {
                break;
            }
            reversed[r_idx++] = input[idx - 1];
        }
    }
    reversed[r_idx] = '\0';
    printf("Reversed: %s\n", reversed);
    
    return 0;
}