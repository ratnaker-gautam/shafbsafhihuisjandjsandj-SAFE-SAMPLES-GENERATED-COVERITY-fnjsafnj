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
    if (len == 0 || (len == 1 && input[0] == '\n')) {
        fprintf(stderr, "Empty input\n");
        return EXIT_FAILURE;
    }
    
    if (len >= sizeof(input)) {
        fprintf(stderr, "Input too long\n");
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
    
    printf("Trimmed: \"%s\"\n", p);
    
    size_t word_count = 0;
    int in_word = 0;
    char *current = p;
    
    while (*current) {
        if (!isspace((unsigned char)*current)) {
            if (!in_word) {
                word_count++;
                in_word = 1;
            }
        } else {
            in_word = 0;
        }
        current++;
    }
    
    printf("Word count: %zu\n", word_count);
    
    char *start = p;
    char *end = p + strlen(p) - 1;
    
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
    
    printf("Reversed: \"%s\"\n", p);
    
    size_t char_count = 0;
    size_t digit_count = 0;
    size_t alpha_count = 0;
    
    current = p;
    while (*current) {
        char_count++;
        if (isdigit((unsigned char)*current)) {
            digit_count++;
        }
        if (isalpha((unsigned char)*current)) {
            alpha_count++;
        }
        current++;
    }
    
    printf("Characters: %zu\n", char_count);
    printf("Digits: %zu\n", digit_count);
    printf("Letters: %zu\n", alpha_count);
    
    return EXIT_SUCCESS;
}