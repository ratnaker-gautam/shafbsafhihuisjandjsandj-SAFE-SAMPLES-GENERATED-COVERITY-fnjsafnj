//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_palindrome_recursive(const char *str, int start, int end) {
    if (start >= end) {
        return 1;
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
        printf("Empty string provided.\n");
        return 1;
    }
    if (len >= 255) {
        printf("Input too long.\n");
        return 1;
    }
    char work_str[256];
    strncpy(work_str, input, sizeof(work_str));
    work_str[sizeof(work_str) - 1] = '\0';
    int palindrome = is_palindrome_recursive(work_str, 0, (int)strlen(work_str) - 1);
    printf("Is palindrome: %s\n", palindrome ? "Yes" : "No");
    int vowels = count_vowels_recursive(work_str, 0);
    printf("Number of vowels: %d\n", vowels);
    reverse_string_recursive(work_str, 0, (int)strlen(work_str) - 1);
    printf("Reversed string: %s\n", work_str);
    return 0;
}