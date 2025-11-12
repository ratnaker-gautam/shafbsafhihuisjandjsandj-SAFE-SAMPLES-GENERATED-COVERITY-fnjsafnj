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
    
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }
    
    if (*start == '\0') {
        fprintf(stderr, "Error: String contains only whitespace\n");
        return 1;
    }
    
    char *last_char = input + len - 1;
    while (last_char > start && isspace((unsigned char)*last_char)) {
        last_char--;
    }
    
    size_t trimmed_len = (size_t)(last_char - start) + 1;
    if (trimmed_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Error: Trimmed string too long\n");
        return 1;
    }
    
    char trimmed[MAX_INPUT_LEN];
    char *r = trimmed;
    char *s = start;
    
    while (s <= last_char) {
        *r++ = *s++;
    }
    *r = '\0';
    
    printf("Trimmed: %s\n", trimmed);
    printf("Trimmed length: %zu\n", trimmed_len);

    char reversed[MAX_INPUT_LEN];
    char *rev_ptr = reversed;
    char *src_ptr = trimmed + trimmed_len - 1;
    
    if (trimmed_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Error: String too long for reversal\n");
        return 1;
    }
    
    for (size_t i = 0; i < trimmed_len; i++) {
        *rev_ptr++ = *src_ptr--;
    }
    *rev_ptr = '\0';
    
    printf("Reversed: %s\n", reversed);

    char *word_start = trimmed;
    char *word_end = trimmed;
    int word_count = 0;
    
    while (*word_start) {
        while (*word_start && isspace((unsigned char)*word_start)) {
            word_start++;
        }
        
        if (*word_start == '\0') {
            break;
        }
        
        word_end = word_start;
        while (*word_end && !isspace((unsigned char)*word_end)) {
            word_end++;
        }
        
        word_count++;
        word_start = word_end;
    }
    
    printf("Word count: %d\n", word_count);

    char uppercase[MAX_INPUT_LEN];
    char *up_ptr = uppercase;
    char *orig_ptr = trimmed;
    
    if (trimmed_len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Error: String too long for conversion\n");
        return 1;
    }
    
    while (*orig_ptr) {
        *up_ptr++ = (char)toupper((unsigned char)*orig_ptr++);
    }
    *up_ptr = '\0';
    
    printf("Uppercase: %s\n", uppercase);

    return 0;
}