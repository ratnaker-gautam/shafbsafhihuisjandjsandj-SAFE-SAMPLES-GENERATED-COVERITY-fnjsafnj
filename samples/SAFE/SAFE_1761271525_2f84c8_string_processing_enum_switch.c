//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_WORD_COUNT
};

void to_uppercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int is_palindrome(const char *str, size_t len) {
    for (size_t i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

size_t count_words(const char *str, size_t len) {
    size_t count = 0;
    int in_word = 0;
    
    for (size_t i = 0; i < len; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        }
    }
    return count;
}

int main(void) {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
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
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    if (len >= sizeof(buffer)) {
        fprintf(stderr, "Input string too long\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("0: Convert to uppercase\n");
    printf("1: Convert to lowercase\n");
    printf("2: Reverse string\n");
    printf("3: Check if palindrome\n");
    printf("4: Count words\n");
    printf("Enter choice (0-4): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid choice input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    size_t buf_len = strlen(buffer);
    
    switch (choice) {
        case OP_UPPERCASE:
            to_uppercase(buffer, buf_len);
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(buffer, buf_len);
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverse_string(buffer, buf_len);
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_PALINDROME_CHECK:
            if (is_palindrome(buffer, buf_len)) {
                printf("The string is a palindrome\n");
            } else {
                printf("The string is not a palindrome\n");
            }
            break;
            
        case OP_WORD_COUNT:
            printf("Word count: %zu\n", count_words(buffer, buf_len));
            break;
            
        default:
            fprintf(stderr, "Invalid operation choice\n");
            return 1;
    }
    
    return 0;
}