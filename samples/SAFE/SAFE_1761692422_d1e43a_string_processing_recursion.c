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

int is_valid_input(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0 || len >= MAX_LENGTH) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) {
            return 0;
        }
    }
    
    return 1;
}

void process_string(char *str) {
    if (str == NULL || strlen(str) == 0) {
        return;
    }
    
    size_t len = strlen(str);
    reverse_string_recursive(str, 0, (int)len - 1);
    
    for (size_t i = 0; i < len; i++) {
        if (islower((unsigned char)str[i])) {
            str[i] = (char)toupper((unsigned char)str[i]);
        } else if (isupper((unsigned char)str[i])) {
            str[i] = (char)tolower((unsigned char)str[i]);
        }
    }
}

int main(void) {
    char input[MAX_LENGTH];
    char working_copy[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
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
        fprintf(stderr, "Invalid input: must be 1-255 printable characters\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(working_copy)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    strncpy(working_copy, input, sizeof(working_copy) - 1);
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    process_string(working_copy);
    
    printf("Original: %s\n", input);
    printf("Processed: %s\n", working_copy);
    
    return 0;
}