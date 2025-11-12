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
    char processed[256];
    
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
        printf("Empty string entered.\n");
        return 1;
    }
    
    if (len >= sizeof(processed)) {
        printf("Input too long for processing.\n");
        return 1;
    }
    
    strcpy(processed, input);
    
    printf("Original: %s\n", input);
    
    remove_non_alpha_recursive(processed, 0, 0);
    printf("Alpha only: %s\n", processed);
    
    int vowel_count = count_vowels_recursive(processed, 0);
    printf("Vowel count: %d\n", vowel_count);
    
    int palindrome = is_palindrome_recursive(processed, 0, strlen(processed) - 1);
    printf("Is palindrome: %s\n", palindrome ? "Yes" : "No");
    
    reverse_string_recursive(processed, 0, strlen(processed) - 1);
    printf("Reversed: %s\n", processed);
    
    return 0;
}