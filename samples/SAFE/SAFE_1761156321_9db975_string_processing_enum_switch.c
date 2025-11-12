//DeepSeek-V3 V2.5 Category: Safe ; Style: enum_switch ; Variation: string_processing
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

enum StringOperation {
    OP_UPPERCASE,
    OP_LOWERCASE,
    OP_REVERSE,
    OP_COUNT_WORDS,
    OP_COUNT_VOWELS,
    OP_EXIT
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

size_t count_vowels(const char *str, size_t len) {
    size_t count = 0;
    const char *vowels = "AEIOUaeiou";
    
    for (size_t i = 0; i < len; i++) {
        for (size_t j = 0; vowels[j] != '\0'; j++) {
            if (str[i] == vowels[j]) {
                count++;
                break;
            }
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    enum StringOperation op;
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string entered.\n");
        return 1;
    }
    
    char working_copy[256];
    if (len >= sizeof(working_copy)) {
        printf("String too long.\n");
        return 1;
    }
    
    while (1) {
        printf("\nChoose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Count vowels\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please enter 1-6.\n");
            continue;
        }
        
        op = (enum StringOperation)(choice - 1);
        
        switch (op) {
            case OP_UPPERCASE:
                strcpy(working_copy, buffer);
                to_uppercase(working_copy, len);
                printf("Uppercase: %s\n", working_copy);
                break;
                
            case OP_LOWERCASE:
                strcpy(working_copy, buffer);
                to_lowercase(working_copy, len);
                printf("Lowercase: %s\n", working_copy);
                break;
                
            case OP_REVERSE:
                strcpy(working_copy, buffer);
                reverse_string(working_copy, len);
                printf("Reversed: %s\n", working_copy);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(buffer, len));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(buffer, len));
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