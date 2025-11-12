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

void remove_non_alpha(char *str) {
    if (str == NULL) return;
    
    char *read_ptr = str;
    char *write_ptr = str;
    
    while (*read_ptr != '\0') {
        if (isalpha((unsigned char)*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

void to_uppercase(char *str) {
    if (str == NULL) return;
    
    char *ptr = str;
    while (*ptr != '\0') {
        *ptr = toupper((unsigned char)*ptr);
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
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original: %s\n", input);
    
    remove_non_alpha(processed);
    printf("Alpha only: %s\n", processed);
    
    to_uppercase(processed);
    printf("Uppercase: %s\n", processed);
    
    size_t processed_len = strlen(processed);
    reverse_string(processed, processed_len);
    printf("Reversed: %s\n", processed);
    
    return EXIT_SUCCESS;
}