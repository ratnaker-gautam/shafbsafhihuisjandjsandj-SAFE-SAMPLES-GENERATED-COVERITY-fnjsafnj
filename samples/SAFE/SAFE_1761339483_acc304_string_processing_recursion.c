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

int count_vowels_recursive(const char *str, int index) {
    if (str[index] == '\0') {
        return 0;
    }
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    return is_vowel + count_vowels_recursive(str, index + 1);
}

void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) {
        return;
    }
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}

int main() {
    char input[256];
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    if (len >= sizeof(input)) {
        printf("Input too long.\n");
        return 1;
    }
    
    char working_copy[256];
    strncpy(working_copy, input, sizeof(working_copy));
    working_copy[sizeof(working_copy) - 1] = '\0';
    
    int palindrome = is_palindrome_recursive(working_copy, 0, (int)strlen(working_copy) - 1);
    printf("Is palindrome (letters only, case-insensitive): %s\n", palindrome ? "Yes" : "No");
    
    int vowels = count_vowels_recursive(input, 0);
    printf("Number of vowels: %d\n", vowels);
    
    char reverse_copy[256];
    strncpy(reverse_copy, input, sizeof(reverse_copy));
    reverse_copy[sizeof(reverse_copy) - 1] = '\0';
    reverse_string_recursive(reverse_copy, 0, (int)strlen(reverse_copy) - 1);
    printf("Reversed string: %s\n", reverse_copy);
    
    return 0;
}