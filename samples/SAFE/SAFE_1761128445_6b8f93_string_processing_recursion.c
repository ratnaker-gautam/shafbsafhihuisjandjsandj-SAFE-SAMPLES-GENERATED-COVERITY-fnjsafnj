//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int check_palindrome(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0) {
        return 1;
    }
    
    if (len > 1000) {
        return 0;
    }
    
    return is_palindrome_recursive(str, 0, (int)len - 1);
}

void remove_newline(char *str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(void) {
    char input[1002];
    
    printf("Enter a string to check if it's a palindrome (letters only, ignoring case and non-letters):\n");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    remove_newline(input);
    
    if (strlen(input) == 0) {
        printf("Empty string is considered a palindrome.\n");
        return 0;
    }
    
    if (strlen(input) >= 1000) {
        printf("Input too long. Maximum 1000 characters allowed.\n");
        return 1;
    }
    
    int result = check_palindrome(input);
    
    if (result) {
        printf("The string \"%s\" is a palindrome.\n", input);
    } else {
        printf("The string \"%s\" is not a palindrome.\n", input);
    }
    
    return 0;
}