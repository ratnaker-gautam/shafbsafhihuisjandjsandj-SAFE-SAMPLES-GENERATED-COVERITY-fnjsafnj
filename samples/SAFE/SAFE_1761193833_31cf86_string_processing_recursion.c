//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowels_recursive(const char *str, int index) {
    if (str == NULL || index < 0) {
        return 0;
    }
    
    if (str[index] == '\0') {
        return 0;
    }
    
    char c = tolower((unsigned char)str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    return is_vowel + count_vowels_recursive(str, index + 1);
}

int is_valid_string(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0 || len > 1000) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint((unsigned char)str[i])) {
            return 0;
        }
    }
    
    return 1;
}

void remove_trailing_newline(char *str) {
    if (str == NULL) return;
    
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

int main(void) {
    char input[1002];
    
    printf("Enter a string (max 1000 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    remove_trailing_newline(input);
    
    if (!is_valid_string(input)) {
        printf("Invalid input. Please enter a printable string between 1 and 1000 characters.\n");
        return 1;
    }
    
    int vowel_count = count_vowels_recursive(input, 0);
    
    printf("The string contains %d vowels.\n", vowel_count);
    
    return 0;
}