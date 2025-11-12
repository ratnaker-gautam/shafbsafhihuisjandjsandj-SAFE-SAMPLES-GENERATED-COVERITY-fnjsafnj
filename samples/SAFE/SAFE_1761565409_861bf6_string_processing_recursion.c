//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH 256

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

void remove_non_alpha_recursive(char *str, int read_pos, int write_pos) {
    if (str[read_pos] == '\0') {
        str[write_pos] = '\0';
        return;
    }
    
    if (isalpha(str[read_pos])) {
        str[write_pos] = tolower(str[read_pos]);
        remove_non_alpha_recursive(str, read_pos + 1, write_pos + 1);
    } else {
        remove_non_alpha_recursive(str, read_pos + 1, write_pos);
    }
}

int count_vowels_recursive(const char *str, int pos) {
    if (str[pos] == '\0') {
        return 0;
    }
    
    char c = tolower(str[pos]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    return is_vowel + count_vowels_recursive(str, pos + 1);
}

int main() {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
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
        printf("Empty string provided.\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    remove_non_alpha_recursive(processed, 0, 0);
    
    printf("Original: %s\n", input);
    printf("Alpha only: %s\n", processed);
    
    int vowel_count = count_vowels_recursive(processed, 0);
    printf("Vowel count: %d\n", vowel_count);
    
    int palindrome = is_palindrome_recursive(processed, 0, strlen(processed) - 1);
    printf("Is palindrome: %s\n", palindrome ? "Yes" : "No");
    
    return 0;
}