//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

int is_valid_string(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_LENGTH) return 0;
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
    if (tolower(str[start]) != tolower(str[end])) return 0;
    return recursive_palindrome_check(str, start + 1, end - 1);
}

void process_string(char *input) {
    if (!is_valid_string(input)) {
        printf("Invalid input string\n");
        return;
    }
    
    char buffer[MAX_LENGTH];
    strncpy(buffer, input, MAX_LENGTH - 1);
    buffer[MAX_LENGTH - 1] = '\0';
    
    printf("Original: %s\n", buffer);
    
    int len = strlen(buffer);
    recursive_reverse(buffer, 0, len - 1);
    printf("Reversed: %s\n", buffer);
    
    int is_palindrome = recursive_palindrome_check(input, 0, len - 1);
    printf("Palindrome: %s\n", is_palindrome ? "Yes" : "No");
    
    int vowel_count = 0;
    for (int i = 0; i < len; i++) {
        char c = tolower(buffer[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            vowel_count++;
        }
    }
    printf("Vowels in reversed string: %d\n", vowel_count);
}

int main() {
    char input[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
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
    
    process_string(input);
    
    return 0;
}