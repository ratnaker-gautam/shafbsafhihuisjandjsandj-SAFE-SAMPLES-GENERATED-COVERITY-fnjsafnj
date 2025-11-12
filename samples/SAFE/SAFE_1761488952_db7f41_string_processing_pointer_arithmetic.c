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

void remove_non_alpha(char *str, size_t len) {
    if (str == NULL || len == 0) {
        return;
    }
    char *read_ptr = str;
    char *write_ptr = str;
    while (*read_ptr && (size_t)(read_ptr - str) < len) {
        if (isalpha(*read_ptr)) {
            *write_ptr = *read_ptr;
            write_ptr++;
        }
        read_ptr++;
    }
    *write_ptr = '\0';
}

int main(void) {
    char input[MAX_INPUT_LEN];
    char buffer[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return EXIT_FAILURE;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_input(input, input_len)) {
        fprintf(stderr, "Invalid input\n");
        return EXIT_FAILURE;
    }
    
    printf("Original: %s\n", input);
    printf("Length: %zu\n", input_len);
    
    if (strcpy(buffer, input) == NULL) {
        fprintf(stderr, "Error copying string\n");
        return EXIT_FAILURE;
    }
    
    reverse_string(buffer, strlen(buffer));
    printf("Reversed: %s\n", buffer);
    
    if (strcpy(buffer, input) == NULL) {
        fprintf(stderr, "Error copying string\n");
        return EXIT_FAILURE;
    }
    
    remove_non_alpha(buffer, strlen(buffer));
    printf("Alpha only: %s\n", buffer);
    
    if (strcpy(buffer, input) == NULL) {
        fprintf(stderr, "Error copying string\n");
        return EXIT_FAILURE;
    }
    
    char *ptr = buffer;
    while (*ptr) {
        if (islower(*ptr)) {
            *ptr = toupper(*ptr);
        }
        ptr++;
    }
    printf("Uppercase: %s\n", buffer);
    
    return EXIT_SUCCESS;
}