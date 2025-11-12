//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_string(const char *str, size_t len) {
    if (str == NULL || len == 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void recursive_reverse(char *str, size_t left, size_t right) {
    if (left >= right) return;
    char temp = str[left];
    str[left] = str[right];
    str[right] = temp;
    recursive_reverse(str, left + 1, right - 1);
}

int recursive_palindrome_check(const char *str, size_t left, size_t right) {
    if (left >= right) return 1;
    if (str[left] != str[right]) return 0;
    return recursive_palindrome_check(str, left + 1, right - 1);
}

int main() {
    char input[256];
    printf("Enter a string (max 255 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_string(input, len)) {
        fprintf(stderr, "Invalid input string\n");
        return 1;
    }
    
    if (len == 0) {
        printf("Empty string\n");
        return 0;
    }
    
    char reversed[256];
    if (len >= sizeof(reversed)) {
        fprintf(stderr, "String too long\n");
        return 1;
    }
    
    strcpy(reversed, input);
    recursive_reverse(reversed, 0, len - 1);
    
    printf("Original: %s\n", input);
    printf("Reversed: %s\n", reversed);
    
    int is_palindrome = recursive_palindrome_check(input, 0, len - 1);
    printf("Palindrome: %s\n", is_palindrome ? "Yes" : "No");
    
    return 0;
}