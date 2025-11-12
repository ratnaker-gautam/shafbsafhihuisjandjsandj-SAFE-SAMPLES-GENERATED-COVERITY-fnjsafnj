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
    
    for (int i = 0; i < len; i++) {
        if (islower((unsigned char)str[i])) {
            str[i] = toupper((unsigned char)str[i]);
        } else if (isupper((unsigned char)str[i])) {
            str[i] = tolower((unsigned char)str[i]);
        }
    }
}

int main() {
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
    if (strlen(input) >= MAX_LENGTH) {
        printf("Input too long\n");
        return 1;
    }
    strncpy(working_copy, input, MAX_LENGTH - 1);
    working_copy[MAX_LENGTH - 1] = '\0';
    
    process_string(working_copy);
    
    if (working_copy[0] == '\0') {
        printf("Processing resulted in empty string\n");
    } else {
        printf("Processed string: %s\n", working_copy);
    }
    
    return 0;
}