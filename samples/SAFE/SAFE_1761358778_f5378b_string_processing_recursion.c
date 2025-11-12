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
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    printf("Original string: %s\n", input);
    
    remove_non_alpha_recursive(processed, 0, 0);
    printf("Alpha characters only: %s\n", processed);
    
    int vowel_count = count_vowels_recursive(processed, 0);
    printf("Number of vowels: %d\n", vowel_count);
    
    char palindrome_check[256];
    strncpy(palindrome_check, processed, sizeof(palindrome_check) - 1);
    palindrome_check[sizeof(palindrome_check) - 1] = '\0';
    
    int is_pal = is_palindrome_recursive(palindrome_check, 0, strlen(palindrome_check) - 1);
    printf("Is palindrome: %s\n", is_pal ? "Yes" : "No");
    
    char reversed[256];
    strncpy(reversed, input, sizeof(reversed) - 1);
    reversed[sizeof(reversed) - 1] = '\0';
    reverse_string_recursive(reversed, 0, strlen(reversed) - 1);
    printf("Reversed original: %s\n", reversed);
    
    return 0;
}