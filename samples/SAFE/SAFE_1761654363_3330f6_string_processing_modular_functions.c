//DeepSeek-V3 V2.5 Category: Safe ; Style: modular_functions ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_INPUT_LEN 1024

int validate_input(const char *input) {
    if (input == NULL) return 0;
    size_t len = strlen(input);
    if (len == 0 || len >= MAX_INPUT_LEN) return 0;
    return 1;
}

void remove_whitespace(char *str) {
    if (str == NULL) return;
    char *dst = str;
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            *dst++ = *str;
        }
        str++;
    }
    *dst = '\0';
}

int count_vowels(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    while (*str) {
        char c = tolower((unsigned char)*str);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
        str++;
    }
    return count;
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int is_palindrome(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        if (str[i] != str[len - i - 1]) {
            return 0;
        }
    }
    return 1;
}

void process_string(char *input) {
    if (!validate_input(input)) {
        printf("Invalid input\n");
        return;
    }

    char buffer[MAX_INPUT_LEN];
    strncpy(buffer, input, MAX_INPUT_LEN - 1);
    buffer[MAX_INPUT_LEN - 1] = '\0';

    printf("Original: %s\n", buffer);
    
    int vowels = count_vowels(buffer);
    printf("Vowel count: %d\n", vowels);
    
    remove_whitespace(buffer);
    printf("Without whitespace: %s\n", buffer);
    
    reverse_string(buffer);
    printf("Reversed: %s\n", buffer);
    
    if (is_palindrome(buffer)) {
        printf("The reversed string is a palindrome\n");
    } else {
        printf("The reversed string is not a palindrome\n");
    }
}

int main() {
    char input[MAX_INPUT_LEN];
    
    printf("Enter a string (max %d characters): ", MAX_INPUT_LEN - 1);
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