//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowel_groups(const char *str, int index, int in_vowel_group) {
    if (str[index] == '\0') {
        return in_vowel_group ? 1 : 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    if (is_vowel && !in_vowel_group) {
        return 1 + count_vowel_groups(str, index + 1, 1);
    } else if (is_vowel && in_vowel_group) {
        return count_vowel_groups(str, index + 1, 1);
    } else {
        return count_vowel_groups(str, index + 1, 0);
    }
}

int validate_input(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len == 0 || len > 1000) {
        return 0;
    }
    
    for (size_t i = 0; i < len; i++) {
        if (!isalpha(str[i]) && !isspace(str[i])) {
            return 0;
        }
    }
    
    return 1;
}

int main() {
    char input[1001];
    
    printf("Enter a string (letters and spaces only, max 1000 chars): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    } else if (len == sizeof(input) - 1) {
        printf("Input too long\n");
        return 1;
    }
    
    if (!validate_input(input)) {
        printf("Invalid input: must contain only letters and spaces, 1-1000 characters\n");
        return 1;
    }
    
    int groups = count_vowel_groups(input, 0, 0);
    printf("Number of vowel groups: %d\n", groups);
    
    return 0;
}