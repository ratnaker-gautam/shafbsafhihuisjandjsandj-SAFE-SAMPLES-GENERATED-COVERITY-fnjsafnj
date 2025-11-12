//DeepSeek-V3 V2.5 Category: Safe ; Style: pointer_arithmetic ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_INPUT_LEN 1024

void reverse_string(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    
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

void remove_non_alpha(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (read_ptr < str + len) {
        if (isalpha(*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void to_uppercase(char *str, size_t len) {
    if (str == NULL || len == 0) return;
    
    char *ptr = str;
    while (ptr < str + len && *ptr != '\0') {
        *ptr = toupper(*ptr);
        ptr++;
    }
}

int main(void) {
    char input[MAX_INPUT_LEN + 1];
    char processed[MAX_INPUT_LEN + 1];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (input_len == 0) {
        fprintf(stderr, "Empty input string\n");
        return EXIT_FAILURE;
    }
    
    if (input_len > MAX_INPUT_LEN) {
        fprintf(stderr, "Input too long\n");
        return EXIT_FAILURE;
    }
    
    strncpy(processed, input, MAX_INPUT_LEN);
    processed[MAX_INPUT_LEN] = '\0';
    
    size_t processed_len = strlen(processed);
    
    printf("Original: %s\n", processed);
    
    remove_non_alpha(processed, processed_len);
    processed_len = strlen(processed);
    
    if (processed_len == 0) {
        fprintf(stderr, "No alphabetic characters found\n");
        return EXIT_FAILURE;
    }
    
    printf("Alpha only: %s\n", processed);
    
    to_uppercase(processed, processed_len);
    printf("Uppercase: %s\n", processed);
    
    reverse_string(processed, processed_len);
    printf("Reversed: %s\n", processed);
    
    return EXIT_SUCCESS;
}