//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_LENGTH) return 0;
    return 1;
}

void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}

void process_string(char *str) {
    if (!is_valid_input(str)) return;
    
    int len = strlen(str);
    if (len == 0) return;
    
    for (int i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) {
            str[0] = '\0';
            return;
        }
    }
    
    reverse_string_recursive(str, 0, len - 1);
}

int main(void) {
    char input[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input\n");
        return 1;
    }
    
    char working_copy[MAX_LENGTH];
    if (strlen(input) >= sizeof(working_copy)) {
        printf("Input too long\n");
        return 1;
    }
    strcpy(working_copy, input);
    
    process_string(working_copy);
    
    if (working_copy[0] == '\0') {
        printf("Invalid characters in input\n");
        return 1;
    }
    
    printf("Original: %s\n", input);
    printf("Reversed: %s\n", working_copy);
    
    return 0;
}