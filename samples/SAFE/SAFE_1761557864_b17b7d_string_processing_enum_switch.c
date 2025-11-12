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
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

size_t count_words(const char *str) {
    if (str == NULL) return 0;
    size_t count = 0;
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

size_t count_vowels(const char *str) {
    if (str == NULL) return 0;
    size_t count = 0;
    for (size_t i = 0; str[i] != '\0'; i++) {
        char c = tolower((unsigned char)str[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
            count++;
        }
    }
    return count;
}

int main(void) {
    char input[256];
    char buffer[256];
    int choice;
    
    printf("String Processing Tool\n");
    printf("Available operations:\n");
    printf("1. Convert to uppercase\n");
    printf("2. Convert to lowercase\n");
    printf("3. Reverse string\n");
    printf("4. Count words\n");
    printf("5. Count vowels\n");
    printf("6. Exit\n");
    
    while (1) {
        printf("\nEnter your choice (1-6): ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        
        if (choice < 1 || choice > 6) {
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
            continue;
        }
        
        enum StringOperation op = choice - 1;
        
        if (op == OP_EXIT) {
            printf("Goodbye!\n");
            break;
        }
        
        printf("Enter a string (max 255 characters): ");
        while (getchar() != '\n');
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }
        
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        
        if (strlen(input) == 0) {
            printf("Empty string provided.\n");
            continue;
        }
        
        strncpy(buffer, input, sizeof(buffer) - 1);
        buffer[sizeof(buffer) - 1] = '\0';
        
        switch (op) {
            case OP_UPPERCASE:
                to_uppercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_LOWERCASE:
                to_lowercase(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_REVERSE:
                reverse_string(buffer);
                printf("Result: %s\n", buffer);
                break;
                
            case OP_COUNT_WORDS:
                printf("Word count: %zu\n", count_words(buffer));
                break;
                
            case OP_COUNT_VOWELS:
                printf("Vowel count: %zu\n", count_vowels(buffer));
                break;
                
            default:
                break;
        }
    }
    
    return 0;
}