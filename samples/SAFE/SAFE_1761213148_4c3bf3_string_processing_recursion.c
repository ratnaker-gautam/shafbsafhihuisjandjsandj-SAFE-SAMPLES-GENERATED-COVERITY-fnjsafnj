//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_string(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    if (len == 0 || len > 1000) return 0;
    for (size_t i = 0; i < len; i++) {
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

void process_string(char *str) {
    if (!is_valid_string(str)) {
        printf("Invalid input string\n");
        return;
    }
    
    printf("Original: %s\n", str);
    
    int vowels = count_vowels_recursive(str, 0);
    printf("Vowel count: %d\n", vowels);
    
    size_t len = strlen(str);
    reverse_string_recursive(str, 0, len - 1);
    printf("Reversed: %s\n", str);
    
    reverse_string_recursive(str, 0, len - 1);
    printf("Restored: %s\n", str);
}

int main() {
    char input[1001];
    
    printf("Enter a string (max 1000 characters): ");
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