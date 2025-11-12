//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

int is_palindrome_recursive(const char *str, int start, int end) {
    if (start >= end) {
        return 1;
    }
    
    if (!isalpha(str[start])) {
        return is_palindrome_recursive(str, start + 1, end);
    }
    
    if (!isalpha(str[end])) {
        return is_palindrome_recursive(str, start, end - 1);
    }
    
    if (tolower(str[start]) != tolower(str[end])) {
        return 0;
    }
    
    return is_palindrome_recursive(str, start + 1, end - 1);
}

void remove_newline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(void) {
    char input[MAX_LENGTH];
    
    printf("Enter a string to check if it's a palindrome (max %d characters): ", MAX_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    remove_newline(input);
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        printf("Empty string is considered a palindrome.\n");
        return 0;
    }
    
    if (input_len >= MAX_LENGTH - 1) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    int result = is_palindrome_recursive(input, 0, (int)input_len - 1);
    
    if (result) {
        printf("The string \"%s\" is a palindrome.\n", input);
    } else {
        printf("The string \"%s\" is not a palindrome.\n", input);
    }
    
    return 0;
}