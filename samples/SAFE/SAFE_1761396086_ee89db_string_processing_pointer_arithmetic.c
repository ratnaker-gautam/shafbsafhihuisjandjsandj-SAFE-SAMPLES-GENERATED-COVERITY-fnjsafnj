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
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) {
        fprintf(stderr, "Invalid input length\n");
        return EXIT_FAILURE;
    }
    
    if (input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        fprintf(stderr, "Empty input string\n");
        return EXIT_FAILURE;
    }
    
    p = input;
    while (*p && isspace((unsigned char)*p)) {
        p++;
    }
    
    if (*p == '\0') {
        fprintf(stderr, "Input contains only whitespace\n");
        return EXIT_FAILURE;
    }
    
    q = input + len - 1;
    while (q > p && isspace((unsigned char)*q)) {
        *q = '\0';
        q--;
    }
    
    char processed[MAX_INPUT_LEN];
    char *dest = processed;
    char *src = p;
    int word_count = 0;
    int in_word = 0;
    
    while (*src && (size_t)(dest - processed) < sizeof(processed) - 1) {
        if (!isspace((unsigned char)*src)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
            *dest = *src;
            dest++;
        } else {
            if (in_word) {
                *dest = ' ';
                dest++;
                in_word = 0;
            }
        }
        src++;
    }
    
    if (dest > processed && *(dest - 1) == ' ') {
        dest--;
    }
    *dest = '\0';
    
    if (strlen(processed) == 0) {
        fprintf(stderr, "No valid words found\n");
        return EXIT_FAILURE;
    }
    
    char reversed[MAX_INPUT_LEN];
    char *rev_ptr = reversed;
    char *word_start = processed;
    char *word_end = processed;
    
    while (*word_end) {
        while (*word_end && !isspace((unsigned char)*word_end)) {
            word_end++;
        }
        
        char *temp = word_end - 1;
        while (temp >= word_start && (size_t)(rev_ptr - reversed) < sizeof(reversed) - 1) {
            *rev_ptr = *temp;
            rev_ptr++;
            temp--;
        }
        
        if (*word_end && (size_t)(rev_ptr - reversed) < sizeof(reversed) - 1) {
            *rev_ptr = ' ';
            rev_ptr++;
        }
        
        if (*word_end) {
            word_end++;
        }
        word_start = word_end;
    }
    
    if (rev_ptr > reversed && *(rev_ptr - 1) == ' ') {
        rev_ptr--;
    }
    *rev_ptr = '\0';
    
    printf("Original: '%s'\n", p);
    printf("Processed: '%s'\n", processed);
    printf("Reversed words: '%s'\n", reversed);
    printf("Word count: %d\n", word_count);
    
    return EXIT_SUCCESS;
}