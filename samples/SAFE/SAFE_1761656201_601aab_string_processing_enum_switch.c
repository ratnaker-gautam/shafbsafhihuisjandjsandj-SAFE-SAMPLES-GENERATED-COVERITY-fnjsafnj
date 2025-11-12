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

void to_uppercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int is_palindrome(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int count_words(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int in_word = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
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
    }
    
    if (strlen(input) == 0) {
        fprintf(stderr, "Empty string provided\n");
        return 1;
    }
    
    printf("Choose operation:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Check if palindrome\n");
    printf("5. Count words\n");
    printf("Enter choice (1-5): ");
    
    if (scanf("%d", &choice) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }
    
    if (choice < 1 || choice > 5) {
        fprintf(stderr, "Choice must be between 1 and 5\n");
        return 1;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOperation op = choice - 1;
    
    switch (op) {
        case OP_UPPERCASE:
            to_uppercase(buffer);
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            to_lowercase(buffer);
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverse_string(buffer);
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_PALINDROME_CHECK:
            if (is_palindrome(buffer)) {
                printf("The string is a palindrome\n");
            } else {
                printf("The string is not a palindrome\n");
            }
            break;
            
        case OP_WORD_COUNT:
            printf("Word count: %d\n", count_words(buffer));
            break;
            
        default:
            fprintf(stderr, "Invalid operation\n");
            return 1;
    }
    
    return 0;
}