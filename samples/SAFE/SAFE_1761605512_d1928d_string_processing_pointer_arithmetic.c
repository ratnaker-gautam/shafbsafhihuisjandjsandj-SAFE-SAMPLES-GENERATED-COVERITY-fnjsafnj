//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main() {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != EOF && c != '\n' && p < end) {
        *p++ = (char)c;
        len++;
    }
    *p = '\0';

    if (len == 0) {
        printf("No input provided.\n");
        return 1;
    }

    printf("Original: %s\n", input);
    printf("Length: %zu\n", len);

    char *start = input;
    char *q = input;
    
    while (*start) {
        if (isalnum((unsigned char)*start)) {
            *q++ = *start;
        }
        start++;
    }
    *q = '\0';
    
    len = q - input;
    printf("Alphanumeric only: %s\n", input);
    printf("New length: %zu\n", len);

    if (len > 0) {
        char *left = input;
        char *right = input + len - 1;
        
        while (left < right) {
            char temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
        
        printf("Reversed: %s\n", input);
    }

    char *search = input;
    int digit_count = 0;
    
    while (*search) {
        if (isdigit((unsigned char)*search)) {
            digit_count++;
        }
        search++;
    }
    
    printf("Digits found: %d\n", digit_count);

    if (len > 0) {
        char *upper = input;
        
        while (*upper) {
            if (isalpha((unsigned char)*upper)) {
                *upper = (char)toupper((unsigned char)*upper);
            }
            upper++;
        }
        
        printf("Uppercase: %s\n", input);
    }

    return 0;
}