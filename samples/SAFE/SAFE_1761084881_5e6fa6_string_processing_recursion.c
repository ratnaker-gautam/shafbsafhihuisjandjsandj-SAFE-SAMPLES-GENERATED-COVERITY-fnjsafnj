//DeepSeek-V3 SAFE v1.4 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowel_groups(const char *str, int index, int in_group) {
    if (str[index] == '\0') {
        return 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    if (is_vowel && !in_group) {
        return 1 + count_vowel_groups(str, index + 1, 1);
    } else if (is_vowel && in_group) {
        return count_vowel_groups(str, index + 1, 1);
    } else {
        return count_vowel_groups(str, index + 1, 0);
    }
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

int is_valid_input(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0 || len > 1000) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) {
            return 0;
        }
    }
    
    return 1;
}

int main(void) {
    char input[1001];
    
    printf("Enter a string (max 1000 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (!is_valid_input(input)) {
        printf("Invalid input: string must be 1-1000 printable characters\n");
        return 1;
    }
    
    int vowel_groups = count_vowel_groups(input, 0, 0);
    printf("Number of vowel groups: %d\n", vowel_groups);
    
    char reversed[1001];
    strcpy(reversed, input);
    reverse_string_recursive(reversed, 0, (int)strlen(reversed) - 1);
    printf("Reversed string: %s\n", reversed);
    
    return 0;
}