//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

int is_valid_input(const char *str, size_t len) {
    if (str == NULL || len == 0 || len >= MAX_INPUT_LEN) {
        return 0;
    }
    const char *p = str;
    while (*p) {
        if (!isprint(*p)) {
            return 0;
        }
        p++;
    }
    return 1;
}

void reverse_string(char *str, size_t len) {
    if (str == NULL || len <= 1) {
        return;
    }
    char *start = str;
    char *end = str + len - 1;
    while (start < end) {
        char temp = *start;
        *start = *end;
        *end = temp;
        start++;
        end--;
    }
}

void remove_vowels(char *str, size_t len) {
    if (str == NULL || len == 0) {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr) {
        char c = *read_ptr;
        if (c != 'a' && c != 'e' && c != 'i' && c != 'o' && c != 'u' &&
            c != 'A' && c != 'E' && c != 'I' && c != 'O' && c != 'U') {
            *write_ptr = c;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void count_char_types(const char *str, int *alpha, int *digit, int *other) {
    if (str == NULL || alpha == NULL || digit == NULL || other == NULL) {
        return;
    }
    *alpha = 0;
    *digit = 0;
    *other = 0;
    const char *p = str;
    while (*p) {
        if (isalpha(*p)) {
            (*alpha)++;
        } else if (isdigit(*p)) {
            (*digit)++;
        } else {
            (*other)++;
        }
        p++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char processed[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    if (len == 0) {
        printf("Empty string\n");
        return EXIT_SUCCESS;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original: %s\n", processed);
    
    reverse_string(processed, strlen(processed));
    printf("Reversed: %s\n", processed);
    
    remove_vowels(processed, strlen(processed));
    printf("Without vowels: %s\n", processed);
    
    int alpha_count, digit_count, other_count;
    count_char_types(processed, &alpha_count, &digit_count, &other_count);
    printf("Character types - Alphabetic: %d, Digits: %d, Other: %d\n", 
           alpha_count, digit_count, other_count);
    
    return EXIT_SUCCESS;
}