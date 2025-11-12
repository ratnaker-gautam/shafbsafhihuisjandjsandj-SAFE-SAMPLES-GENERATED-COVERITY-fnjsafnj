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

int main() {
    char input[256];
    char processed[256];
    
    printf("Enter a string to check if it's a palindrome: ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string is considered a palindrome\n");
        return 0;
    }
    
    if (len >= sizeof(processed)) {
        fprintf(stderr, "Input too long\n");
        return 1;
    }
    
    strncpy(processed, input, sizeof(processed) - 1);
    processed[sizeof(processed) - 1] = '\0';
    
    remove_non_alpha_recursive(processed, 0, 0);
    
    int processed_len = strlen(processed);
    if (processed_len == 0) {
        printf("No alphabetic characters found\n");
        return 0;
    }
    
    int result = is_palindrome_recursive(processed, 0, processed_len - 1);
    
    printf("Original: \"%s\"\n", input);
    printf("Processed: \"%s\"\n", processed);
    if (result) {
        printf("This is a palindrome\n");
    } else {
        printf("This is not a palindrome\n");
    }
    
    return 0;
}