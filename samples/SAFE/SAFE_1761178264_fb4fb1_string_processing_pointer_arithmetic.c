//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p = input;
    char *end = input + MAX_INPUT_LEN - 1;
    int c;
    size_t len = 0;

    printf("Enter a string (max %d chars): ", MAX_INPUT_LEN - 1);
    
    while ((c = getchar()) != EOF && c != '\n' && p < end) {
        *p++ = (char)c;
        len++;
    }
    
    if (p == input) {
        fprintf(stderr, "Error: No input provided\n");
        return 1;
    }
    
    *p = '\0';
    
    if (len == 0) {
        fprintf(stderr, "Error: Empty string\n");
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
        
        left = input;
        right = input + len - 1;
        
        while (left < right) {
            char temp = *left;
            *left = *right;
            *right = temp;
            left++;
            right--;
        }
        
        printf("Restored: %s\n", input);
    }
    
    size_t word_count = 0;
    char *token = input;
    int in_word = 0;
    
    while (*token) {
        if (isalnum((unsigned char)*token)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        token++;
    }
    
    printf("Word count: %zu\n", word_count);
    
    char search[MAX_INPUT_LEN];
    printf("Enter substring to search: ");
    
    p = search;
    while ((c = getchar()) != EOF && c != '\n' && p < search + MAX_INPUT_LEN - 1) {
        *p++ = (char)c;
    }
    *p = '\0';
    
    if (p > search) {
        size_t search_len = p - search;
        size_t found = 0;
        
        char *haystack = input;
        while (*haystack && (haystack + search_len) <= (input + len)) {
            char *needle = search;
            char *h = haystack;
            
            while (*needle && *h == *needle) {
                needle++;
                h++;
            }
            
            if (!*needle) {
                found++;
            }
            
            haystack++;
        }
        
        printf("Substring found %zu times\n", found);
    }
    
    return 0;
}