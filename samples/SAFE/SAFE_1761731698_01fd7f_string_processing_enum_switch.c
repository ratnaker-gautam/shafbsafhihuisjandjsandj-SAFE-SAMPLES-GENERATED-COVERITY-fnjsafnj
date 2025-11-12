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

void to_uppercase(char *str) {
    if (!str) return;
    for (size_t i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

void to_lowercase(char *str) {
    if (!str) return;
    for (size_t i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void reverse_string(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    for (size_t i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

size_t count_words(const char *str) {
    if (!str) return 0;
    size_t count = 0;
    int in_word = 0;
    for (size_t i = 0; str[i]; i++) {
        if (isspace((unsigned char)str[i])) {
            in_word = 0;
        } else if (!in_word) {
            count++;
            in_word = 1;
        }
    }
    return count;
}

size_t count_vowels(const char *str) {
    if (!str) return 0;
    size_t count = 0;
    for (size_t i = 0; str[i]; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    char buffer[256];
    char working_copy[256];
    int choice;
    
    printf("Enter a string (max 255 characters): ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    if (strlen(buffer) == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    while (1) {
        printf("\nOriginal string: %s\n", buffer);
        printf("Choose operation:\n");
        printf("1. Convert to uppercase\n");
        printf("2. Convert to lowercase\n");
        printf("3. Reverse string\n");
        printf("4. Count words\n");
        printf("5. Count vowels\n");
        printf("6. Exit\n");
        printf("Enter choice (1-6): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input\n");
            while (getchar() != '\n');
            continue;
        }
        
        while (getchar() != '\n');
        
        if (choice < 1 || choice > 6) {
            printf("Invalid choice\n");
            continue;
        }
        
        enum StringOperation op = choice - 1;
        
        switch (op) {
            case OP_UPPERCASE:
                strncpy(working_copy, buffer, sizeof(working_copy) - 1);
                working_copy[sizeof(working_copy) - 1] = '\0';
                to_uppercase(working_copy);
                printf("Uppercase: %s\n", working_copy);
                break;
                
            case OP_LOWERCASE:
                strncpy(working_copy, buffer, sizeof(working_copy) - 1);
                working_copy[sizeof(working_copy) - 1] = '\0';
                to_lowercase(working_copy);
                printf("Lowercase: %s\n", working_copy);
                break;
                
            case OP_REVERSE:
                strncpy(working_copy, buffer, sizeof(working_copy) - 1);
                working_copy[sizeof(working_copy) - 1] = '\0';
                reverse_string(working_copy);
                printf("Reversed: %s\n", working_copy);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(buffer));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(buffer));
                break;
                
            case OP_EXIT:
                printf("Goodbye!\n");
                return 0;
                
            default:
                printf("Invalid operation\n");
                break;
        }
    }
    
    return 0;
}