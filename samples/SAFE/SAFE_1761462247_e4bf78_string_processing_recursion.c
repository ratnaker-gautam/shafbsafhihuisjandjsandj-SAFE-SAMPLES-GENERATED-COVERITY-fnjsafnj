//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_string(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > 1000) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void recursive_reverse(char *str, int start, int end) {
    if (start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    recursive_reverse(str, start + 1, end - 1);
}

int recursive_palindrome_check(const char *str, int start, int end) {
    if (start >= end) return 1;
    if (str[start] != str[end]) return 0;
    return recursive_palindrome_check(str, start + 1, end - 1);
}

int main() {
    char input[1001];
    printf("Enter a string (max 1000 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_string(input)) {
        printf("Invalid input string.\n");
        return 1;
    }
    
    char reversed[1001];
    strcpy(reversed, input);
    recursive_reverse(reversed, 0, len - 1);
    
    printf("Original: %s\n", input);
    printf("Reversed: %s\n", reversed);
    
    int is_palindrome = recursive_palindrome_check(input, 0, len - 1);
    if (is_palindrome) {
        printf("The string is a palindrome.\n");
    } else {
        printf("The string is not a palindrome.\n");
    }
    
    return 0;
}