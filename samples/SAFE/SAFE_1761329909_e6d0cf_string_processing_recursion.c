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

void process_string(const char *input) {
    if (input == NULL) {
        printf("Error: Null input string\n");
        return;
    }
    size_t len = strlen(input);
    if (len == 0) {
        printf("Error: Empty input string\n");
        return;
    }
    if (len > 1000) {
        printf("Error: Input string too long\n");
        return;
    }
    
    int result = is_palindrome_recursive(input, 0, (int)len - 1);
    printf("Input: \"%s\"\n", input);
    printf("Is palindrome (ignoring non-letters): %s\n", result ? "Yes" : "No");
}

int main() {
    char input[1001];
    
    printf("Enter a string to check if it's a palindrome (max 1000 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    process_string(input);
    return 0;
}