//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) {
        return;
    }
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}

int is_valid_input(const char *input) {
    if (input == NULL) {
        return 0;
    }
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_LENGTH) {
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)input[i])) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    char input[MAX_LENGTH];
    char buffer[MAX_LENGTH];
    
    printf("Enter a string to reverse (max %d characters): ", MAX_LENGTH - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        fprintf(stderr, "Invalid input: must be printable characters and 1-%d characters long\n", MAX_LENGTH - 1);
        return 1;
    }
    
    if (len == 0) {
        fprintf(stderr, "Input string cannot be empty\n");
        return 1;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    printf("Original: %s\n", buffer);
    reverse_string_recursive(buffer, 0, (int)strlen(buffer) - 1);
    printf("Reversed: %s\n", buffer);
    
    return 0;
}