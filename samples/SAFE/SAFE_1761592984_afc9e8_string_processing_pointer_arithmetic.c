//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT 1024

int main() {
    char input[MAX_INPUT];
    char output[MAX_INPUT];
    
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
    
    if (len >= MAX_INPUT - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    char *src = input;
    char *dst = output;
    char *end = input + len;
    
    while (src < end) {
        if (isalpha((unsigned char)*src)) {
            if (islower((unsigned char)*src)) {
                *dst = toupper((unsigned char)*src);
            } else {
                *dst = tolower((unsigned char)*src);
            }
            dst++;
        } else if (isdigit((unsigned char)*src)) {
            *dst = *src;
            dst++;
        }
        src++;
    }
    
    *dst = '\0';
    
    size_t out_len = dst - output;
    if (out_len == 0) {
        printf("No alphanumeric characters found\n");
        return 0;
    }
    
    printf("Processed string: %s\n", output);
    
    char *rev_ptr = output + out_len - 1;
    printf("Reversed: ");
    while (rev_ptr >= output) {
        putchar(*rev_ptr);
        rev_ptr--;
    }
    putchar('\n');
    
    char *search_ptr = output;
    int vowel_count = 0;
    while (*search_ptr) {
        char c = tolower((unsigned char)*search_ptr);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        }
        search_ptr++;
    }
    
    printf("Vowel count: %d\n", vowel_count);
    
    return 0;
}