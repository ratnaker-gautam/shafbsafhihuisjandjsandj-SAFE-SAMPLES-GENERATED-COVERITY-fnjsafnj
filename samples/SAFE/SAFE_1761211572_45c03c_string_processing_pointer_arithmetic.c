//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int main(void) {
    char input[MAX_INPUT_LEN];
    char *p, *q;
    size_t len;
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty string\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *processed = malloc(len + 1);
    if (processed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    p = input;
    q = processed;
    
    while (*p != '\0') {
        if (isalpha((unsigned char)*p)) {
            *q = toupper((unsigned char)*p);
            q++;
        }
        p++;
    }
    *q = '\0';
    
    size_t processed_len = q - processed;
    
    if (processed_len == 0) {
        printf("No alphabetic characters found\n");
        free(processed);
        return 0;
    }
    
    printf("Original: %s\n", input);
    printf("Processed (uppercase letters only): %s\n", processed);
    printf("Processed length: %zu\n", processed_len);
    
    char *reverse = malloc(processed_len + 1);
    if (reverse == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(processed);
        return 1;
    }
    
    p = processed + processed_len - 1;
    q = reverse;
    
    while (p >= processed) {
        *q = *p;
        q++;
        p--;
    }
    *q = '\0';
    
    printf("Reversed: %s\n", reverse);
    
    int is_palindrome = 1;
    p = processed;
    q = processed + processed_len - 1;
    
    while (p < q) {
        if (*p != *q) {
            is_palindrome = 0;
            break;
        }
        p++;
        q--;
    }
    
    printf("Is palindrome: %s\n", is_palindrome ? "yes" : "no");
    
    free(processed);
    free(reverse);
    return 0;
}