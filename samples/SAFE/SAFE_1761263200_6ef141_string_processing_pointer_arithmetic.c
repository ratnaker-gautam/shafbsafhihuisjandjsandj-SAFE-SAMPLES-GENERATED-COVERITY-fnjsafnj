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
    if (str == NULL || len < 2) {
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

void remove_whitespace(char *str) {
    if (str == NULL) {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr) {
        if (!isspace((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void count_character_types(const char *str, int *alpha, int *digit, int *other) {
    if (str == NULL || alpha == NULL || digit == NULL || other == NULL) {
        return;
    }
    *alpha = 0;
    *digit = 0;
    *other = 0;
    const char *p = str;
    while (*p) {
        if (isalpha((unsigned char)*p)) {
            (*alpha)++;
        } else if (isdigit((unsigned char)*p)) {
            (*digit)++;
        } else {
            (*other)++;
        }
        p++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char buffer[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input, len)) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    printf("Original string: \"%s\"\n", input);
    printf("Length: %zu\n", len);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    reverse_string(buffer, strlen(buffer));
    printf("Reversed: \"%s\"\n", buffer);
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    remove_whitespace(buffer);
    printf("Without whitespace: \"%s\"\n", buffer);
    
    int alpha_count, digit_count, other_count;
    count_character_types(input, &alpha_count, &digit_count, &other_count);
    printf("Character types - Alphabetic: %d, Digits: %d, Other: %d\n", 
           alpha_count, digit_count, other_count);
    
    char *ptr = input;
    printf("Characters: ");
    while (*ptr) {
        printf("%c ", *ptr);
        ptr++;
    }
    printf("\n");
    
    return 0;
}