//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowels_recursive(const char *str, int index) {
    if (str == NULL || index < 0) {
        return -1;
    }
    
    if (str[index] == '\0') {
        return 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    int count = count_vowels_recursive(str, index + 1);
    if (count < 0) {
        return -1;
    }
    
    return count + (is_vowel ? 1 : 0);
}

int count_vowels(const char *str) {
    if (str == NULL) {
        return -1;
    }
    return count_vowels_recursive(str, 0);
}

void reverse_string_recursive(char *str, int start, int end) {
    if (str == NULL || start < 0 || end < 0 || start >= end) {
        return;
    }
    
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    
    reverse_string_recursive(str, start + 1, end - 1);
}

void reverse_string(char *str) {
    if (str == NULL) {
        return;
    }
    
    int len = strlen(str);
    if (len <= 1) {
        return;
    }
    
    reverse_string_recursive(str, 0, len - 1);
}

int is_palindrome_recursive(const char *str, int start, int end) {
    if (str == NULL || start < 0 || end < 0) {
        return 0;
    }
    
    if (start >= end) {
        return 1;
    }
    
    if (tolower(str[start]) != tolower(str[end])) {
        return 0;
    }
    
    return is_palindrome_recursive(str, start + 1, end - 1);
}

int is_palindrome(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    int len = strlen(str);
    if (len <= 1) {
        return 1;
    }
    
    return is_palindrome_recursive(str, 0, len - 1);
}

int main() {
    char input[256];
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    int vowel_count = count_vowels(input);
    if (vowel_count < 0) {
        printf("Error counting vowels\n");
        return 1;
    }
    
    printf("Number of vowels: %d\n", vowel_count);
    
    char reverse_copy[256];
    if (strlen(input) >= sizeof(reverse_copy)) {
        printf("String too long for processing\n");
        return 1;
    }
    strcpy(reverse_copy, input);
    reverse_string(reverse_copy);
    printf("Reversed string: %s\n", reverse_copy);
    
    int palindrome = is_palindrome(input);
    printf("Is palindrome: %s\n", palindrome ? "Yes" : "No");
    
    return 0;
}