//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) {
        return;
    }
    
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    
    reverse_string_recursive(str, start + 1, end - 1);
}

int count_vowels_recursive(const char *str, int index) {
    if (str[index] == '\0') {
        return 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    return is_vowel + count_vowels_recursive(str, index + 1);
}

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

void process_string_operations(const char *input) {
    char buffer[MAX_LENGTH];
    int len = strlen(input);
    
    if (len >= MAX_LENGTH) {
        printf("Input too long for processing.\n");
        return;
    }
    
    strncpy(buffer, input, MAX_LENGTH - 1);
    buffer[MAX_LENGTH - 1] = '\0';
    
    printf("Original: %s\n", buffer);
    
    int vowel_count = count_vowels_recursive(buffer, 0);
    printf("Vowel count: %d\n", vowel_count);
    
    reverse_string_recursive(buffer, 0, len - 1);
    printf("Reversed: %s\n", buffer);
    
    int palindrome_result = is_palindrome_recursive(input, 0, len - 1);
    printf("Is palindrome: %s\n", palindrome_result ? "Yes" : "No");
}

int main() {
    char input[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty input provided.\n");
        return 1;
    }
    
    process_string_operations(input);
    
    return 0;
}