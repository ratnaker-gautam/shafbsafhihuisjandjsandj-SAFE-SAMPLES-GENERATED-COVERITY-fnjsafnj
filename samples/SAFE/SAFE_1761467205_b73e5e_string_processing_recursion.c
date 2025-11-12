//DeepSeek-V3 V2.5 Category: Safe ; Style: recursion ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int is_valid_string(const char *str, size_t len) {
    if (str == NULL || len == 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isprint(str[i])) return 0;
    }
    return 1;
}

int count_substring_recursive(const char *str, const char *sub, size_t str_len, size_t sub_len, size_t pos) {
    if (str == NULL || sub == NULL || sub_len == 0 || pos > str_len - sub_len) {
        return 0;
    }
    int found = 0;
    if (strncmp(str + pos, sub, sub_len) == 0) {
        found = 1;
    }
    return found + count_substring_recursive(str, sub, str_len, sub_len, pos + 1);
}

void reverse_string_recursive(char *str, size_t start, size_t end) {
    if (str == NULL || start >= end) return;
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverse_string_recursive(str, start + 1, end - 1);
}

int main() {
    char input[256];
    char substring[256];
    
    printf("Enter a main string (max 255 chars): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        fprintf(stderr, "Error reading input\n");
        return 1;
    }
    
    size_t input_len = strlen(input);
    if (input_len > 0 && input[input_len - 1] == '\n') {
        input[input_len - 1] = '\0';
        input_len--;
    }
    
    if (!is_valid_string(input, input_len)) {
        fprintf(stderr, "Invalid input string\n");
        return 1;
    }
    
    printf("Enter a substring to search for (max 255 chars): ");
    if (fgets(substring, sizeof(substring), stdin) == NULL) {
        fprintf(stderr, "Error reading substring\n");
        return 1;
    }
    
    size_t sub_len = strlen(substring);
    if (sub_len > 0 && substring[sub_len - 1] == '\n') {
        substring[sub_len - 1] = '\0';
        sub_len--;
    }
    
    if (!is_valid_string(substring, sub_len)) {
        fprintf(stderr, "Invalid substring\n");
        return 1;
    }
    
    if (sub_len == 0) {
        fprintf(stderr, "Substring cannot be empty\n");
        return 1;
    }
    
    if (sub_len > input_len) {
        printf("Substring is longer than main string. Count: 0\n");
    } else {
        int count = count_substring_recursive(input, substring, input_len, sub_len, 0);
        printf("Substring occurs %d times\n", count);
    }
    
    char reversed[256];
    if (input_len >= sizeof(reversed)) {
        fprintf(stderr, "Input too long for reversal buffer\n");
        return 1;
    }
    strncpy(reversed, input, sizeof(reversed));
    reversed[sizeof(reversed) - 1] = '\0';
    
    if (input_len > 0) {
        reverse_string_recursive(reversed, 0, input_len - 1);
        printf("Reversed string: %s\n", reversed);
    } else {
        printf("Reversed string: (empty)\n");
    }
    
    return 0;
}