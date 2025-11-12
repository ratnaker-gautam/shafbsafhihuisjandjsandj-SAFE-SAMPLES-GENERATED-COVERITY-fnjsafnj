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
        str[write_pos] = str[read_pos];
        remove_non_alpha_recursive(str, read_pos + 1, write_pos + 1);
    } else {
        remove_non_alpha_recursive(str, read_pos + 1, write_pos);
    }
}

int main(void) {
    char input[MAX_LENGTH];
    char processed[MAX_LENGTH];
    
    printf("Enter a string (max %d characters): ", MAX_LENGTH - 1);
    
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
    
    remove_non_alpha_recursive(processed, 0, 0);
    
    printf("Original: %s\n", input);
    printf("Alpha only: %s\n", processed);
    
    if (strlen(processed) > 0) {
        int result = is_palindrome_recursive(processed, 0, strlen(processed) - 1);
        printf("Is palindrome (letters only, case insensitive): %s\n", result ? "Yes" : "No");
    } else {
        printf("No alphabetic characters to check for palindrome\n");
    }
    
    return 0;
}