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
        printf("Empty string provided\n");
        return 0;
    }
    
    if (len >= MAX_LEN - 1) {
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
            
            for (size_t j = 0; j < word_len; j++) {
                processed[p_idx++] = input[word_start + j];
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
    
    printf("Processed string: %s\n", processed);
    
    size_t temp_idx = 0;
    for (size_t j = 0; j < p_idx; j++) {
        if (temp_idx >= sizeof(temp) - 1) {
            break;
        }
        if (isalpha((unsigned char)processed[j])) {
            temp[temp_idx++] = toupper((unsigned char)processed[j]);
        }
    }
    temp[temp_idx] = '\0';
    
    printf("Uppercase letters: %s\n", temp);
    
    int count = 0;
    for (size_t j = 0; temp[j] != '\0'; j++) {
        if (temp[j] == 'A' || temp[j] == 'E' || temp[j] == 'I' || 
            temp[j] == 'O' || temp[j] == 'U') {
            count++;
        }
    }
    
    printf("Number of vowels in uppercase: %d\n", count);
    
    return 0;
}