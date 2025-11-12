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
    OP_COUNT_WORDS,
    OP_EXIT
};

void toUpperCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverseString(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - 1 - i];
        str[len - 1 - i] = temp;
    }
}

int isPalindrome(const char *str) {
    int len = strlen(str);
    for (int i = 0; i < len / 2; i++) {
        if (str[i] != str[len - 1 - i]) {
            return 0;
        }
    }
    return 1;
}

int countWords(const char *str) {
    int count = 0;
    int inWord = 0;
    for (int i = 0; str[i]; i++) {
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
    
    while (1) {
        printf("Enter a string (max 255 chars): ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string entered.\n");
            continue;
        }
        
        printf("Choose operation:\n");
        printf("0: Convert to uppercase\n");
        printf("1: Convert to lowercase\n");
        printf("2: Reverse string\n");
        printf("3: Check if palindrome\n");
        printf("4: Count words\n");
        printf("5: Exit\n");
        printf("Enter choice: ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        while (getchar() != '\n');
        
        if (choice < 0 || choice > 5) {
            printf("Invalid choice.\n");
            continue;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        switch ((enum StringOperation)choice) {
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
                if (isPalindrome(input)) {
                    printf("The string is a palindrome.\n");
                } else {
                    printf("The string is not a palindrome.\n");
                }
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %d\n", countWords(input));
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid operation.\n");
                break;
        }
    }
    
    return 0;
}