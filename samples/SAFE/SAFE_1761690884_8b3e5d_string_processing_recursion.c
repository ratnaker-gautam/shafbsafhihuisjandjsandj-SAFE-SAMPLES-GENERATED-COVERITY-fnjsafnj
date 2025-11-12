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
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    printf("Original string: %s\n", input);
    
    int palindrome = is_palindrome_recursive(input, 0, strlen(input) - 1);
    printf("Is palindrome (ignoring non-letters): %s\n", palindrome ? "Yes" : "No");
    
    int vowels = count_vowels_recursive(input, 0);
    printf("Number of vowels: %d\n", vowels);
    
    char reversed[256];
    strncpy(reversed, input, sizeof(reversed) - 1);
    reversed[sizeof(reversed) - 1] = '\0';
    reverse_string_recursive(reversed, 0, strlen(reversed) - 1);
    printf("Reversed string: %s\n", reversed);
    
    return 0;
}