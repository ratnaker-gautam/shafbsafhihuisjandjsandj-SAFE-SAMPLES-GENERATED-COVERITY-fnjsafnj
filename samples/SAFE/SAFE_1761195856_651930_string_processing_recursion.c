//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    if (strlen(str) >= MAX_LENGTH) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

void reverse_string_recursive(char *str, int start, int end) {
    if (start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}

int count_vowels_recursive(const char *str, int index) {
    if (str[index] == '\0') return 0;
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    return is_vowel + count_vowels_recursive(str, index + 1);
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
        len--;
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: string too long or contains non-printable characters.\n");
        return 1;
    }
    
    if (len == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    char reversed[MAX_LENGTH];
    strncpy(reversed, input, MAX_LENGTH);
    reversed[MAX_LENGTH - 1] = '\0';
    
    if (len > 0) {
        reverse_string_recursive(reversed, 0, len - 1);
    }
    
    int vowel_count = count_vowels_recursive(input, 0);
    
    printf("Original string: %s\n", input);
    printf("Reversed string: %s\n", reversed);
    printf("Number of vowels: %d\n", vowel_count);
    
    return 0;
}