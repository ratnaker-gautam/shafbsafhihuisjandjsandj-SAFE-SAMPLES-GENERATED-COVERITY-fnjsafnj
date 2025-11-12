//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_PALINDROME_CHECK,
    OP_WORD_COUNT
};

void toUpperCase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void toLowerCase(char *str) {
    if (str == NULL) return;
    for (size_t i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverseString(char *str) {
    if (str == NULL) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int isPalindrome(const char *str) {
    if (str == NULL) return 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int countWords(const char *str) {
    if (str == NULL) return 0;
    int count = 0;
    int inWord = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isspace((unsigned char)str[i])) {
            inWord = 0;
        } else if (!inWord) {
            count++;
            inWord = 1;
        }
    }
    return count;
}

int main() {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(input, sizeof(input), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0';
    }
    
    if (strlen(input) == 0) {
        printf("Empty string provided\n");
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
        printf("Invalid input\n");
        return 1;
    }
    
    while (getchar() != '\n');
    
    if (choice < 1 || choice > 5) {
        printf("Invalid choice\n");
        return 1;
    }
    
    strncpy(buffer, input, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    enum StringOperation op = choice - 1;
    
    switch (op) {
        case OP_UPPERCASE:
            toUpperCase(buffer);
            printf("Uppercase: %s\n", buffer);
            break;
            
        case OP_LOWERCASE:
            toLowerCase(buffer);
            printf("Lowercase: %s\n", buffer);
            break;
            
        case OP_REVERSE:
            reverseString(buffer);
            printf("Reversed: %s\n", buffer);
            break;
            
        case OP_PALINDROME_CHECK:
            if (isPalindrome(buffer)) {
                printf("The string is a palindrome\n");
            } else {
                printf("The string is not a palindrome\n");
            }
            break;
            
        case OP_WORD_COUNT:
            printf("Word count: %d\n", countWords(buffer));
            break;
            
        default:
            printf("Invalid operation\n");
            return 1;
    }
    
    return 0;
}