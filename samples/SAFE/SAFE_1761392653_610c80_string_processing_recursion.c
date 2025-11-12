//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
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

int validate_string(const char *str) {
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

void process_input(const char *input) {
    if (!validate_string(input)) {
        printf("Invalid input string\n");
        return;
    }
    
    int groups = count_vowel_groups(input, 0, 0);
    printf("Input: %s\n", input);
    printf("Number of vowel groups: %d\n", groups);
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
    }
    
    process_input(input);
    
    return 0;
}