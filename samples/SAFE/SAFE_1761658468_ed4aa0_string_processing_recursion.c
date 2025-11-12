//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowel_groups(const char *str, int index, int in_group) {
    if (str == NULL || index < 0) return 0;
    
    if (str[index] == '\0') {
        return in_group ? 1 : 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    if (is_vowel) {
        if (!in_group) {
            return 1 + count_vowel_groups(str, index + 1, 1);
        } else {
            return count_vowel_groups(str, index + 1, 1);
        }
    } else {
        return count_vowel_groups(str, index + 1, 0);
    }
}

void reverse_string_recursive(char *str, int start, int end) {
    if (str == NULL || start < 0 || end < 0 || start >= end) return;
    
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    
    reverse_string_recursive(str, start + 1, end - 1);
}

int is_valid_input(const char *str) {
    if (str == NULL) return 0;
    
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i]) && str[i] != ' ') {
            return 0;
        }
    }
    return 1;
}

int main() {
    char input[256];
    
    printf("Enter a string (letters and spaces only): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: only letters and spaces allowed\n");
        return 1;
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    int vowel_groups = count_vowel_groups(input, 0, 0);
    printf("Number of vowel groups: %d\n", vowel_groups);
    
    char reversed[256];
    strncpy(reversed, input, sizeof(reversed) - 1);
    reversed[sizeof(reversed) - 1] = '\0';
    
    int str_len = strlen(reversed);
    if (str_len > 0) {
        reverse_string_recursive(reversed, 0, str_len - 1);
        printf("Reversed string: %s\n", reversed);
    }
    
    return 0;
}