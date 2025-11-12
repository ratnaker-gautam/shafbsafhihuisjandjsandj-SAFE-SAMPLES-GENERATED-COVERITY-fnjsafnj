//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int count_vowels_recursive(const char *str, int index) {
    if (str == NULL || index < 0) {
        return -1;
    }
    
    if (str[index] == '\0') {
        return 0;
    }
    
    char c = tolower(str[index]);
    int is_vowel = (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
    
    int count = count_vowels_recursive(str, index + 1);
    if (count < 0) {
        return -1;
    }
    
    return count + (is_vowel ? 1 : 0);
}

int is_valid_string(const char *str) {
    if (str == NULL) {
        return 0;
    }
    
    size_t len = strlen(str);
    if (len > 1000) {
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
    char input[1002];
    
    printf("Enter a string (max 1000 characters): ");
    
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (!is_valid_string(input)) {
        fprintf(stderr, "Invalid input string\n");
        return 1;
    }
    
    int vowel_count = count_vowels_recursive(input, 0);
    
    if (vowel_count < 0) {
        fprintf(stderr, "Error processing string\n");
        return 1;
    }
    
    printf("Number of vowels in the string: %d\n", vowel_count);
    
    return 0;
}