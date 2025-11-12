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
    char working_copy[256];
    enum StringOperation choice;
    int valid_input;
    
    printf("Enter a string (max 255 characters): ");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("Error reading input\n");
        return 1;
    }
    
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        printf("Empty string provided\n");
        return 1;
    }
    
    do {
        printf("\nChoose operation:\n");
        printf("0: Convert to uppercase\n");
        printf("1: Convert to lowercase\n");
        printf("2: Reverse string\n");
        printf("3: Count words\n");
        printf("4: Count vowels\n");
        printf("5: Exit\n");
        printf("Enter choice (0-5): ");
        
        valid_input = 0;
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            int temp;
            if (sscanf(buffer, "%d", &temp) == 1) {
                if (temp >= OP_UPPERCASE && temp <= OP_EXIT) {
                    choice = (enum StringOperation)temp;
                    valid_input = 1;
                }
            }
        }
        
        if (!valid_input) {
            printf("Invalid choice. Please enter 0-5.\n");
            continue;
        }
        
        if (choice == OP_EXIT) {
            break;
        }
        
        strncpy(working_copy, buffer, sizeof(working_copy) - 1);
        working_copy[sizeof(working_copy) - 1] = '\0';
        
        switch (choice) {
            case OP_UPPERCASE:
                to_uppercase(working_copy, strlen(working_copy));
                printf("Uppercase: %s\n", working_copy);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(working_copy, strlen(working_copy));
                printf("Lowercase: %s\n", working_copy);
                break;
                
            case OP_REVERSE:
                reverse_string(working_copy, strlen(working_copy));
                printf("Reversed: %s\n", working_copy);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(working_copy, strlen(working_copy)));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(working_copy, strlen(working_copy)));
                break;
                
            default:
                break;
        }
        
    } while (1);
    
    printf("Goodbye!\n");
    return 0;
}