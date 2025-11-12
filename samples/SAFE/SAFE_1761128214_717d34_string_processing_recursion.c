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
    if (pos > str_len - sub_len) return 0;
    if (strncmp(str + pos, sub, sub_len) == 0) {
        return 1 + count_substring_recursive(str, sub, str_len, sub_len, pos + 1);
    }
    return count_substring_recursive(str, sub, str_len, sub_len, pos + 1);
}

int main() {
    char input[256];
    char substring[128];
    
    printf("Enter main string (max 255 chars): ");
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
        fprintf(stderr, "Invalid characters in main string\n");
        return 1;
    }
    
    printf("Enter substring to count (max 127 chars): ");
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
        fprintf(stderr, "Invalid characters in substring\n");
        return 1;
    }
    
    if (sub_len == 0) {
        fprintf(stderr, "Substring cannot be empty\n");
        return 1;
    }
    
    if (sub_len > input_len) {
        printf("Substring count: 0\n");
        return 0;
    }
    
    int count = count_substring_recursive(input, substring, input_len, sub_len, 0);
    printf("Substring count: %d\n", count);
    
    return 0;
}