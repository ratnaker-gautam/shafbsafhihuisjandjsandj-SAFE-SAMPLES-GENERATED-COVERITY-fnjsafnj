//DeepSeek-V3 V2.5 Category: Safe ; Style: loop_variants ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LEN 256

int main() {
    char input[MAX_LEN];
    char processed[MAX_LEN * 2];
    
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
        fprintf(stderr, "Empty input string\n");
        return 1;
    }
    
    if (len >= MAX_LEN - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    size_t p_idx = 0;
    size_t i = 0;
    
    while (i < len) {
        char current = input[i];
        
        if (isalpha(current)) {
            if (p_idx < sizeof(processed) - 3) {
                processed[p_idx++] = toupper(current);
                processed[p_idx++] = '-';
            }
        } else if (isdigit(current)) {
            if (p_idx < sizeof(processed) - 2) {
                processed[p_idx++] = '#';
            }
        } else {
            if (p_idx < sizeof(processed) - 1) {
                processed[p_idx++] = current;
            }
        }
        
        i++;
    }
    
    if (p_idx > 0 && processed[p_idx - 1] == '-') {
        p_idx--;
    }
    
    processed[p_idx] = '\0';
    
    size_t j = 0;
    size_t word_count = 0;
    char temp[MAX_LEN * 2];
    
    for (j = 0; j < p_idx; j++) {
        temp[j] = processed[j];
        if (processed[j] == ' ') {
            word_count++;
        }
    }
    temp[j] = '\0';
    
    size_t k = 0;
    size_t char_count = 0;
    
    do {
        if (temp[k] != '\0') {
            char_count++;
        }
        k++;
    } while (temp[k] != '\0' && k < sizeof(temp));
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", processed);
    printf("Character count: %zu\n", char_count);
    printf("Word count: %zu\n", word_count + 1);
    
    size_t m = 0;
    size_t n = p_idx - 1;
    int is_palindrome = 1;
    
    while (m < n) {
        if (processed[m] != processed[n]) {
            is_palindrome = 0;
            break;
        }
        m++;
        n--;
    }
    
    if (is_palindrome) {
        printf("The processed string is a palindrome\n");
    } else {
        printf("The processed string is not a palindrome\n");
    }
    
    return 0;
}