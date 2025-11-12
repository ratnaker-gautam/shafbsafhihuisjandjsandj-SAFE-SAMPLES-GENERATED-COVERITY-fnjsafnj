//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

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
    char buffer[256];
    
    printf("Enter a string: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input.\n");
        return 1;
    }
    
    input[strcspn(input, "\n")] = '\0';
    
    if (strlen(input) == 0) {
        printf("Empty string provided.\n");
        return 1;
    }
    
    if (strlen(input) >= sizeof(buffer)) {
        printf("Input too long.\n");
        return 1;
    }
    
    strcpy(buffer, input);
    remove_non_alpha_recursive(buffer, 0, 0);
    
    printf("Original: %s\n", input);
    printf("Alpha only: %s\n", buffer);
    
    int vowel_count = count_vowels_recursive(input, 0);
    printf("Vowel count: %d\n", vowel_count);
    
    strcpy(buffer, input);
    reverse_string_recursive(buffer, 0, strlen(buffer) - 1);
    printf("Reversed: %s\n", buffer);
    
    int palindrome = is_palindrome_recursive(input, 0, strlen(input) - 1);
    printf("Is palindrome (ignoring non-alpha): %s\n", palindrome ? "Yes" : "No");
    
    return 0;
}