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
    
    if (p == end && c != '\n' && c != EOF) {
        int discard;
        while ((discard = getchar()) != EOF && discard != '\n');
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
    
    while (*start && start < input + MAX_INPUT_LEN) {
        if (isalnum((unsigned char)*start)) {
            *q++ = *start;
        }
        start++;
    }
    *q = '\0';
    
    size_t new_len = q - input;
    printf("Alphanumeric only: %s\n", input);
    printf("New length: %zu\n", new_len);

    if (new_len > 0) {
        char *front = input;
        char *back = input + new_len - 1;
        
        while (front < back) {
            char temp = *front;
            *front = *back;
            *back = temp;
            front++;
            back--;
        }
        
        printf("Reversed: %s\n", input);
    }

    char search[MAX_INPUT_LEN];
    printf("Enter substring to search: ");
    
    p = search;
    while ((c = getchar()) != EOF && c != '\n' && p < search + MAX_INPUT_LEN - 1) {
        *p++ = (char)c;
    }
    *p = '\0';
    
    if (p == search) {
        printf("No search term provided.\n");
        return 1;
    }

    size_t search_len = p - search;
    int found = 0;
    
    if (search_len <= new_len) {
        char *haystack = input;
        char *needle_end = input + new_len - search_len + 1;
        
        while (haystack < needle_end) {
            char *h = haystack;
            char *n = search;
            int match = 1;
            
            while (*n && h < input + MAX_INPUT_LEN) {
                if (*h != *n) {
                    match = 0;
                    break;
                }
                h++;
                n++;
            }
            
            if (match) {
                found = 1;
                break;
            }
            haystack++;
        }
    }

    if (found) {
        printf("Substring found.\n");
    } else {
        printf("Substring not found.\n");
    }

    return 0;
}